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

public:
  Drawer() = default;
  Drawer(const ftxui::Dimensions dimensions);
  int rows() const;
  int cols() const;
  const terminal_webcam::Size size() const;
  void Set(terminal_webcam::Image image);
  void Draw() const;
  void Clear();
  std::string ToString() const;
};
} // namespace pixelator
