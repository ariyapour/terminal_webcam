#include "terminal_webcam/kitty_graphics.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

namespace terminal_webcam {
namespace kitty_graphics {
namespace {

constexpr char kBase64Table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Kitty requires all graphics payloads (pixel data or file paths) to be
// base64 so legacy terminals do not interpret the bytes as control codes.
std::string base64_encode(const unsigned char *data, std::size_t len) {
  std::string out;
  out.reserve(((len + 2) / 3) * 4);
  std::size_t i = 0;
  while (i + 2 < len) {
    const unsigned int n =
        (static_cast<unsigned int>(data[i]) << 16) |
        (static_cast<unsigned int>(data[i + 1]) << 8) |
        static_cast<unsigned int>(data[i + 2]);
    out.push_back(kBase64Table[(n >> 18) & 63]);
    out.push_back(kBase64Table[(n >> 12) & 63]);
    out.push_back(kBase64Table[(n >> 6) & 63]);
    out.push_back(kBase64Table[n & 63]);
    i += 3;
  }
  if (i < len) {
    unsigned int n = static_cast<unsigned int>(data[i]) << 16;
    if (i + 1 < len) {
      n |= static_cast<unsigned int>(data[i + 1]) << 8;
    }
    out.push_back(kBase64Table[(n >> 18) & 63]);
    out.push_back(kBase64Table[(n >> 12) & 63]);
    if (i + 1 < len) {
      out.push_back(kBase64Table[(n >> 6) & 63]);
    } else {
      out.push_back('=');
    }
    out.push_back('=');
  }
  return out;
}

bool looks_like_kitty() {
  // Kitty sets KITTY_WINDOW_ID; TERM is xterm-kitty in a normal Kitty window.
  const char *kitty_window = std::getenv("KITTY_WINDOW_ID");
  if (kitty_window != nullptr && kitty_window[0] != '\0') {
    return true;
  }
  const char *term = std::getenv("TERM");
  return term != nullptr && std::strcmp(term, "xterm-kitty") == 0;
}

// Graphics commands are APC sequences: ESC _ G <control> ; <payload> ESC \
void write_apc(const std::string &control, const std::string &payload) {
  std::cout << "\033_G" << control << ";" << payload << "\033\\";
}

// Direct transfer (t=d) must split base64 into chunks of at most 4096 bytes.
// m=1 means more chunks follow; m=0 is the last chunk. Only the first chunk
// carries width/height/format; later chunks send m (and optionally q) only.
void write_stream_chunks(const std::string &control,
                         const std::string &payload) {
  constexpr std::size_t kMaxChunk = 4096;
  if (payload.empty()) {
    write_apc(control + ",m=0", "");
    return;
  }
  std::size_t offset = 0;
  bool first = true;
  while (offset < payload.size()) {
    const std::size_t remaining = payload.size() - offset;
    const std::size_t chunk_size =
        remaining > kMaxChunk ? kMaxChunk : remaining;
    const bool last = offset + chunk_size >= payload.size();
    std::string chunk_control;
    if (first) {
      chunk_control = control + (last ? ",m=0" : ",m=1");
      first = false;
    } else {
      chunk_control = last ? "m=0" : "m=1";
    }
    write_apc(chunk_control, payload.substr(offset, chunk_size));
    offset += chunk_size;
  }
}

std::string temp_dir() {
  // Kitty will only auto-delete temp-file transfers from known temp dirs
  // (/tmp, /dev/shm, TMPDIR).
  const char *tmpdir = std::getenv("TMPDIR");
  if (tmpdir != nullptr && tmpdir[0] != '\0') {
    return tmpdir;
  }
  return "/tmp";
}

} // namespace

bool is_available() {
  if (!looks_like_kitty()) {
    return false;
  }
  const PixelSize size = pixel_size();
  return size.width > 0 && size.height > 0;
}

PixelSize pixel_size() {
  struct winsize sz {};
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz) != 0) {
    return {};
  }
  return {static_cast<int>(sz.ws_xpixel), static_cast<int>(sz.ws_ypixel)};
}

Session::Session() {
  // Alternate screen, clear, home cursor, hide cursor.
  std::cout << "\033[?1049h\033[2J\033[H\033[?25l";
  std::cout.flush();
}

Session::~Session() {
  // a=d deletes visible placements; d=A also frees stored image data.
  std::cout << "\033_Ga=d\033\\\033_Ga=d,d=A\033\\";
  std::cout << "\033[?25h\033[?1049l";
  std::cout.flush();
}

bool Session::draw_via_temp_file(const std::uint8_t *rgb, int width,
                                 int height) {
  // t=t: write raw RGB to a temp file and send the path. Kitty reads the
  // file itself, which is much cheaper than streaming megabytes over the PTY.
  // The path must contain "tty-graphics-protocol" or Kitty will refuse to
  // delete the file after reading it.
  const std::string pattern =
      temp_dir() + "/terminal-webcam-tty-graphics-protocol-XXXXXX";
  std::vector<char> path(pattern.begin(), pattern.end());
  path.push_back('\0');
  const int fd = mkstemp(path.data());
  if (fd < 0) {
    return false;
  }

  const std::size_t total =
      static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 3;
  std::size_t written = 0;
  while (written < total) {
    const ssize_t n = write(fd, rgb + written, total - written);
    if (n <= 0) {
      close(fd);
      unlink(path.data());
      return false;
    }
    written += static_cast<std::size_t>(n);
  }
  // Flush to disk before telling Kitty to open the file.
  if (fsync(fd) != 0) {
    close(fd);
    unlink(path.data());
    return false;
  }
  if (close(fd) != 0) {
    unlink(path.data());
    return false;
  }

  const std::string encoded =
      base64_encode(reinterpret_cast<const unsigned char *>(path.data()),
                    std::strlen(path.data()));
  // a=T transmit+display, f=24 RGB, t=t temp file, i=1 replace previous
  // frame, q=2 no stdin replies, C=1 do not move the cursor after placing.
  const std::string control = "a=T,f=24,s=" + std::to_string(width) +
                              ",v=" + std::to_string(height) +
                              ",t=t,i=1,q=2,C=1";
  write_apc(control, encoded);
  return true;
}

void Session::draw_via_stream(const std::uint8_t *rgb, int width, int height) {
  // Used when the temp file cannot be written (or Kitty cannot see /tmp,
  // e.g. over SSH). Payload is base64 of the RGB bytes themselves.
  const std::size_t total =
      static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 3;
  const std::string encoded = base64_encode(rgb, total);
  const std::string control = "a=T,f=24,s=" + std::to_string(width) +
                              ",v=" + std::to_string(height) + ",i=1,q=2,C=1";
  write_stream_chunks(control, encoded);
}

bool Session::draw_rgb(const std::uint8_t *rgb, int width, int height) {
  if (rgb == nullptr || width <= 0 || height <= 0) {
    return false;
  }
  if (!draw_via_temp_file(rgb, width, height)) {
    draw_via_stream(rgb, width, height);
  }
  // Place the next frame at the top-left instead of scrolling.
  std::cout << "\033[H";
  std::cout.flush();
  return true;
}

} // namespace kitty_graphics
} // namespace terminal_webcam
