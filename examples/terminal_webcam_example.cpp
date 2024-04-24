#include <iostream>
#include <opencv2/opencv.hpp>
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/color.hpp"
#include "terminal_webcam/drawer.hpp"
#include "terminal_webcam/image.hpp"

int main(int, char**) {
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0); // in linux check $ ls /dev/video0
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    const ftxui::Dimensions dimensions{ftxui::Dimension::Full()};
    terminal_webcam::Drawer drawer{dimensions};
    std::cout<<"Terminal size: "<< dimensions.dimx << "X"<<dimensions.dimy<<std::endl;

    // create a window to display the images from the webcam
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    // array to hold image
    cv::Mat frame;
    
    // display the frame until you press a key
    while (1) {
        // capture the next frame from the webcam
        camera >> frame;
        //Resize the image to the terminal size
        cv::resize(frame, frame, cv::Size(static_cast<int>(ftxui::Dimension::Full().dimx), static_cast<int>(ftxui::Dimension::Full().dimy)), cv::INTER_LINEAR);

        terminal_webcam::Image image(frame);
        drawer.Set(image);
        drawer.Draw();
        drawer.Clear();
    }
    return 0;
}
