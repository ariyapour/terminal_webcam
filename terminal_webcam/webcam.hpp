#pragma once

namespace terminal_webcam {

// Captures from a camera and draws frames in the terminal: Kitty graphics
// when available, otherwise the FTXUI cell-based (pixelated) renderer.
class webcam {
public:
  webcam() = default;
  // Open camera_index (default 0) and run until SIGINT/SIGTERM or a capture
  // error. Returns 1 if the camera cannot be opened, otherwise 0.
  int show(int camera_index = 0);
};

} // namespace terminal_webcam
