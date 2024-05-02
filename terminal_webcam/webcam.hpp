#pragma once
#include "terminal_webcam/drawer.hpp"
#include "ftxui/screen/screen.hpp"
#include <opencv2/opencv.hpp>
namespace terminal_webcam{
    class webcam
    {
    private:
        terminal_webcam::Drawer drawer_{ftxui::Dimension::Full()};
    public:
        webcam() = default;
        int show(int camera_index = 0);
    };
}