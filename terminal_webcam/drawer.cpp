#include "terminal_webcam/drawer.hpp"
#include <algorithm>
#include <iostream>

terminal_webcam::Drawer::Drawer(const ftxui::Dimensions dimensions) {
  if (dimensions.dimx == ftxui::Dimension::Full().dimx &&
      dimensions.dimy == ftxui::Dimension::Full().dimy) {
    rows_ = ftxui::Dimension::Full().dimx;
    cols_ = ftxui::Dimension::Full().dimy;
  } else {
    rows_ = dimensions.dimx;
    cols_ = rows_ * 2;
  }
  screen_ = ftxui::Screen::Create({rows_, cols_});
}

int terminal_webcam::Drawer::rows() const { return rows_; }

int terminal_webcam::Drawer::cols() const { return cols_; }

const terminal_webcam::Size terminal_webcam::Drawer::size() const {
  return terminal_webcam::Size{rows_, cols_};
}

void terminal_webcam::Drawer::Set(terminal_webcam::Image image) {
  for (int i = 0; i < image.rows(); i++) {
    for (int j = 0; j < image.cols(); j++) {
      auto &pixel_left = screen_.PixelAt(j * 2, i);
      pixel_left.background_color = image.at(i, j);
      pixel_left.character = ' ';
      auto &pixel_right = screen_.PixelAt(j * 2 + 1, i);
      pixel_right.background_color = image.at(i, j);
      pixel_right.character = ' ';
    }
  }

  return;
}

void terminal_webcam::Drawer::Draw() const {
  screen_.Print();
  return;
}

void terminal_webcam::Drawer::Clear() {
  //Clear the screen and reset the cursor position
  std::cout<< screen_.ResetPosition();
  return;
}

std::string terminal_webcam::Drawer::ToString() const {
  std::string string_image = screen_.ToString();

  // Remove new line characters from the end of the string
  // TODO: I added removing of the new line characters here since I was getting
  // testing errors in github but not on local machine. I did not perform
  // extensive test when I added this.
  while (!string_image.empty() &&
         (string_image[string_image.size() - 1] == '\r' ||
          string_image[string_image.size() - 1] == '\n' ||
          string_image[string_image.size() - 1] == ' ')) {
    string_image.erase(string_image.size() - 1);
  }

  return string_image;
}