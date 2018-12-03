//
// Created by afterburner on 18-12-3.
//

#include "binarization.h"
#include <opencv2/core.hpp>          // for merge, split
#include <opencv2/imgproc.hpp>       // for blur, cvtColor, equalizeHist

cv::Mat equalize_color_image(cv::Mat input_image){
//    cv::cvtColor(input_image, input_image, cv::COLOR_BGR2YCrCb);
    std::vector<cv::Mat> channels(3);
    cv::split(input_image, channels);
    for (auto& channel : channels) {
        cv::equalizeHist(channel, channel);
    }
    cv::merge(channels, input_image);
    return input_image;
}

cv::Mat binarize(cv::Mat input_image, const float threshold) {
    input_image = equalize_color_image(input_image);
    cv::cvtColor(input_image, input_image, cv::COLOR_BGR2GRAY);
    cv::blur(input_image, input_image, cv::Size(5, 5));
    cv::threshold(input_image, input_image, threshold, 255.0, cv::THRESH_BINARY);
    return input_image;
}

