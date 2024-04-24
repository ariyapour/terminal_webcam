#include "terminal_webcam/image.hpp"
#include <opencv2/core/types.hpp>

terminal_webcam::Image::Image(cv::Mat &image)
    : rows_{image.rows}, cols_{image.cols}, channels_{image.channels()} {
  image_data_.resize(rows_ * cols_ * channels_, ftxui::Color{});
  // Set the image data
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      auto color = image.at<cv::Vec3b>(i, j);
      image_data_.at(channels_ * (i * cols_ + j)) =
          ftxui::Color{color[0], color[1], color[2]};
    }
  }
}

terminal_webcam::Image::Image(terminal_webcam::Size size)
    : rows_{size.rows}, cols_{size.cols} {
  image_data_.resize(rows_ * cols_ * channels_, ftxui::Color{});
};

terminal_webcam::Image::Image(const int rows, const int cols)
    : rows_{rows}, cols_{cols} {
  image_data_.resize(rows_ * cols_ * channels_, ftxui::Color{});
};

terminal_webcam::Image::Image(terminal_webcam::Image &&other_image)
    : rows_{other_image.rows_}, cols_{other_image.cols_},
      channels_{other_image.channels_}, image_data_{other_image.image_data_} {
  other_image.image_data_.clear();
  other_image.rows_ = 0;
  other_image.cols_ = 0;
  other_image.channels_ = 0;
}

terminal_webcam::Image::Image(terminal_webcam::Image &other_image)
    : rows_{other_image.rows_}, cols_{other_image.cols_},
      channels_{other_image.channels_}, image_data_{other_image.image_data_} {}

bool terminal_webcam::Image::empty() const { return image_data_.empty(); }

int terminal_webcam::Image::rows() const { return rows_; }

int terminal_webcam::Image::cols() const { return cols_; }

terminal_webcam::Size terminal_webcam::Image::size() {
  return terminal_webcam::Size{rows_, cols_};
}

const terminal_webcam::Size terminal_webcam::Image::size() const {
  return terminal_webcam::Size{rows_, cols_};
}

ftxui::Color &terminal_webcam::Image::at(int row, int col) {
  return image_data_.at(channels_ * (row * cols_ + col));
}

const ftxui::Color &terminal_webcam::Image::at(int row, int col) const {
  return image_data_.at(channels_ * (row * cols_ + col));
}

terminal_webcam::Image &
terminal_webcam::Image::operator=(terminal_webcam::Image &&other_image) {
  if (this == &other_image) {
    return *this;
  }
  if (!image_data_.empty()) {
    image_data_.clear();
  }

  image_data_ = other_image.image_data_;
  rows_ = other_image.rows_;
  cols_ = other_image.cols_;
  channels_ = other_image.channels_;
  other_image.rows_ = 0;
  other_image.cols_ = 0;
  other_image.channels_ = 0;

  return *this;
}