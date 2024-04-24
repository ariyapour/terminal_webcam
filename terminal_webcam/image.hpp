#pragma once
#include "ftxui/screen/color.hpp"
#include <vector>

namespace terminal_webcam {
struct Size {
  int rows;
  int cols;
};

class Image {
private:
  int rows_{};
  int cols_{};
  int channels_{3};
  std::vector<ftxui::Color> image_data_{};

public:
  Image() = default;
  Image(const terminal_webcam::Size size);
  // Move constructor
  Image(Image &&other_image);
  // copy constructor
  Image(Image &other_image);
  Image(const int rows, const int cols);
  bool empty() const;
  int rows() const;
  int cols() const;
  terminal_webcam::Size size();
  const terminal_webcam::Size size() const;
  const ftxui::Color &at(int row, int col) const;
  ftxui::Color &at(int row, int col);
  // Move assignment operator
  Image &operator=(Image &&other_image);
};

} // namespace pixelator