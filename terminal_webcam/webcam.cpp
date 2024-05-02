#include "terminal_webcam/webcam.hpp"
#include "terminal_webcam/utils.h"
#include "terminal_webcam/image.hpp"
#include <iostream>

int terminal_webcam::webcam::show(int camera_index){
    cv::VideoCapture camera(camera_index);
    if (!camera.isOpened())
    {
        std::cerr<<"ERROR: Could not open camera. Provide another camera index!"<<std::endl;
        return 1;
    }

    cv::Mat frame;

    while (1)
    {
        drawer_.update_drawer_to_full_size();
        camera >> frame;
        frame =  terminal_webcam::utils::resizeAndFillBorders(
            frame, drawer_.rows(), drawer_.cols(),
            cv::Scalar(0, 0, 0), drawer_.chars_in_x(), drawer_.chars_in_y());

        terminal_webcam::Image image(frame);
        drawer_.Set(image);
        drawer_.Draw();
        drawer_.Clear();
    }
    return 0;
}