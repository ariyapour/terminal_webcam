#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

namespace terminal_webcam{
    namespace utils {
        cv::Mat resizeAndFillBorders(const cv::Mat& inputImage, int targetWidth, int targetHeight, const cv::Scalar& fillColor, int drawer_pixels_in_x, int drawer_pixels_in_y) {
            //Check the drawer specific configurations and rescale the target width and height
            targetWidth = targetWidth/drawer_pixels_in_x;
            targetHeight = targetHeight/drawer_pixels_in_y;
            // Calculate aspect ratios of input and target sizes
            double inputAspectRatio = static_cast<double>(inputImage.cols) / inputImage.rows;
            double targetAspectRatio = static_cast<double>(targetWidth) / targetHeight;

            // Create a new image with the target size and fill with the specified color
            cv::Mat outputImage(targetHeight, targetWidth, inputImage.type(), fillColor);

            // Calculate scaling factors for resizing
            double scale = 0.0;
            if (inputAspectRatio > targetAspectRatio) {
                // Resize based on width
                scale = static_cast<double>(targetWidth) / inputImage.cols;
            } else {
                // Resize based on height
                scale = static_cast<double>(targetHeight) / inputImage.rows;
            }

            // Resize the input image while preserving aspect ratio
            cv::Mat resizedImage;
            cv::resize(inputImage, resizedImage, cv::Size(), scale, scale);

            // Calculate the position to place the resized image in the center of the output image
            int xStart = (outputImage.cols - resizedImage.cols) / 2;
            int yStart = (outputImage.rows - resizedImage.rows) / 2;


            // Copy the resized image to the center of the output image
            resizedImage.copyTo(outputImage(cv::Rect(xStart, yStart, resizedImage.cols, resizedImage.rows)));

            return outputImage;
        }
    }
}
