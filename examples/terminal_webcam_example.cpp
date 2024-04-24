#include <iostream>
#include <opencv2/opencv.hpp>
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/color.hpp"

int main(int, char**) {
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0); // in linux check $ ls /dev/video0
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    const ftxui::Dimensions dimensions{ftxui::Dimension::Full()};
    ftxui::Screen screen{ftxui::Screen::Create(dimensions)};

    std::cout<<"Terminal full screen size: "<< dimensions.dimx<<" X "<< dimensions.dimy<<std::endl;

    // create a window to display the images from the webcam
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    // array to hold image
    cv::Mat frame;
    
    // display the frame until you press a key
    while (1) {
        // capture the next frame from the webcam
        camera >> frame;
        // show the image on the window
        cv::resize(frame, frame, cv::Size(static_cast<int>(dimensions.dimx), static_cast<int>(dimensions.dimy)), cv::INTER_LINEAR);
        cv::imshow("Webcam", frame);
        // wait (10ms) for esc key to be pressed to stop
        if (cv::waitKey(10) == 27)
            break;
    }
    return 0;
}
