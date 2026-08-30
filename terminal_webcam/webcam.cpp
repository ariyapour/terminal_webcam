#include "terminal_webcam/webcam.hpp"
#include "terminal_webcam/drawer.hpp"
#include "terminal_webcam/image.hpp"
#include "terminal_webcam/kitty_graphics.hpp"
#include "terminal_webcam/utils.h"

#include "ftxui/screen/screen.hpp"

#include <atomic>
#include <cstdint>
#include <csignal>
#include <iostream>
#include <opencv2/opencv.hpp>

namespace {
std::atomic<bool> g_running{true};

// Only store a flag: signal handlers must stay async-signal-safe so that
// Kitty Session can still run its destructor and delete uploaded images.
void handle_stop_signal(int) { g_running.store(false); }

void install_stop_handlers() {
  std::signal(SIGINT, handle_stop_signal);
  std::signal(SIGTERM, handle_stop_signal);
}
} // namespace

int terminal_webcam::webcam::show(int camera_index) {
  cv::VideoCapture camera(camera_index);
  if (!camera.isOpened()) {
    std::cerr << "ERROR: Could not open camera. Provide another camera index!"
              << std::endl;
    return 1;
  }

  install_stop_handlers();
  g_running.store(true);

  if (kitty_graphics::is_available()) {
    kitty_graphics::Session session;
    cv::Mat frame;
    while (g_running.load()) {
      camera >> frame;
      if (frame.empty()) {
        std::cerr << "Error: Unable to capture frame" << std::endl;
        break;
      }
      const kitty_graphics::PixelSize size = kitty_graphics::pixel_size();
      if (size.width <= 0 || size.height <= 0) {
        continue;
      }
      // 1,1: letterbox to real pixels, not the 2x1 cell mapping used by FTXUI.
      frame = terminal_webcam::utils::resizeAndFillBorders(
          frame, size.width, size.height, cv::Scalar(0, 0, 0), 1, 1);
      cv::Mat rgb;
      cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
      if (!rgb.isContinuous()) {
        rgb = rgb.clone();
      }
      session.draw_rgb(rgb.ptr<std::uint8_t>(), rgb.cols, rgb.rows);
    }
    return 0;
  }

  // Fallback: one colored terminal cell (2 chars wide) per source pixel.
  terminal_webcam::Drawer drawer{ftxui::Dimension::Full()};
  cv::Mat frame;
  while (g_running.load()) {
    drawer.update_drawer_to_full_size();
    camera >> frame;
    if (frame.empty()) {
      std::cerr << "Error: Unable to capture frame" << std::endl;
      break;
    }
    frame = terminal_webcam::utils::resizeAndFillBorders(
        frame, drawer.rows(), drawer.cols(), cv::Scalar(0, 0, 0),
        drawer.chars_in_x(), drawer.chars_in_y());

    terminal_webcam::Image image(frame);
    drawer.Set(image);
    drawer.Draw();
    drawer.Clear();
  }
  return 0;
}
