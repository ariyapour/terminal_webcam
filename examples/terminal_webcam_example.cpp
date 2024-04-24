#include "ftxui/screen/color.hpp"
#include "ftxui/screen/screen.hpp"
#include "terminal_webcam/drawer.hpp"
#include "terminal_webcam/image.hpp"
#include "terminal_webcam/utils.h"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char **) {
  // open the first webcam plugged in the computer
  cv::VideoCapture camera(0); // in linux check $ ls /dev/video0
  if (!camera.isOpened()) {
    std::cerr << "ERROR: Could not open camera" << std::endl;
    return 1;
  }

  const ftxui::Dimensions dimensions{ftxui::Dimension::Full()};
  terminal_webcam::Drawer drawer{dimensions};

  // create a window to display the images from the webcam
  cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

  // array to hold image
  cv::Mat frame;

  // display the frame until you press a key
  while (1) {
    // capture the next frame from the webcam
    camera >> frame;
    // Resize the image to the terminal size
    frame = terminal_webcam::utils::resizeAndFillBorders(
        frame, ftxui::Dimension::Full().dimx, ftxui::Dimension::Full().dimy,
        cv::Scalar(0, 0, 0), drawer.chars_in_x(), drawer.chars_in_y());

    terminal_webcam::Image image(frame);
    drawer.Set(image);
    drawer.Draw();
    drawer.Clear();
  }
  return 0;
}
