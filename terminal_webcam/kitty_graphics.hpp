#pragma once

#include <cstdint>

namespace terminal_webcam {
namespace kitty_graphics {

// Pixel dimensions of the terminal window (not character cells).
struct PixelSize {
  int width{};
  int height{};
};

// True when the process looks like it is running in Kitty and ioctl reports a
// non-zero pixel window size. Used to pick the high-res path vs FTXUI cells.
bool is_available();

// Window size in pixels via TIOCGWINSZ (ws_xpixel / ws_ypixel). Many terminals
// leave these as 0; Kitty fills them in.
PixelSize pixel_size();

// RAII wrapper around a Kitty graphics session: alternate screen, hidden
// cursor, and deletion of uploaded images on destroy.
class Session {
public:
  Session();
  ~Session();
  Session(const Session &) = delete;
  Session &operator=(const Session &) = delete;

  // Display a packed 24-bit RGB buffer (row-major, 3 bytes per pixel).
  // Prefers a temp-file transfer; falls back to chunked stdin streaming.
  bool draw_rgb(const std::uint8_t *rgb, int width, int height);

private:
  bool draw_via_temp_file(const std::uint8_t *rgb, int width, int height);
  void draw_via_stream(const std::uint8_t *rgb, int width, int height);
};

} // namespace kitty_graphics
} // namespace terminal_webcam
