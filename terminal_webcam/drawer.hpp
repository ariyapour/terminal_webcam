#pragma once
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/screen.hpp"
#include "terminal_webcam/image.hpp"
namespace terminal_webcam {
class Drawer {
private:
  ftxui::Screen screen_{0, 0};
  int rows_{};
  int cols_{};
  // This variable represents the number of characters should be filled in X direction to
  // represent a single pixel on terminal.
  int number_of_chars_for_pixels_x_{2};
  // This variable represents the number of characters should be filled in Y direction to
  // represent a single pixel on terminal.
  int number_of_chars_for_pixels_y_{1};

public:
  Drawer() = default;
  Drawer(const ftxui::Dimensions dimensions);
  int rows() const;
  int cols() const;
  int chars_in_x() const;
  int chars_in_y() const;
  void update_drawer_to_full_size();
  const terminal_webcam::Size size() const;
  void Set(terminal_webcam::Image image);
  void Draw() const;
  void Clear();
  std::string ToString() const;
};
} // namespace terminal_webcam
