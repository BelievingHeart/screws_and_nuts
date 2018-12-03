#include "binarization.h"           // for binarize
#include <iostream>                 // for operator<<, cout, ostream
#include <opencv2/core.hpp>         // for RNG_MT19937
#include <opencv2/highgui.hpp>      // for imshow, waitKey, createTrackbar
#include <opencv2/imgproc.hpp>      // for blur, connectedComponentsWithStats

const char *winname = "Image";
cv::Mat threshed;

int main(int argc, const char *argv[]) {
  auto input_image = cv::imread("./screws_nuts.png", cv::IMREAD_ANYCOLOR);
  if (input_image.data == nullptr) {
    std::cout << "Input image not exists.\n";
  }

  //    subtract from background
  const auto subtract_from_background = [](cv::Mat src_) -> cv::Mat {
    cv::Mat background;
    cv::blur(src_, background, cv::Size(src_.cols / 3, src_.cols / 3));
    cv::blur(background, background, cv::Size(src_.cols / 3, src_.cols / 3));
    return cv::abs(src_ - background);
  };
  input_image = subtract_from_background(std::move(input_image));
  //    cv::imshow("After subtraction", input_image);
  //    cv::waitKey(0);

  // set threshold trackbar
  cv::namedWindow(winname, cv::WINDOW_GUI_EXPANDED);
  const auto tweak_threshold = [](const int pos, void *usrdata) {
    threshed = binarize(static_cast<cv::Mat *>(usrdata)->clone(), pos);

    cv::imshow(winname, threshed);
  };
  int _ = 0;
  cv::createTrackbar("Threshold", winname, &_, 255, tweak_threshold,
                     static_cast<void *>(&input_image));

  //    thresh
  auto output_image = binarize(input_image.clone(), 0);
  cv::imshow(winname, output_image);
  cv::waitKey(0);

  //  connected components
  const auto get_components_image = [](cv::Mat src_) {
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    cv::connectedComponentsWithStats(src_, labels, stats, centroids, 8);

    const auto random_color = [](const int i) {
      cv::RNG rng(i);
      return cv::Scalar(rng.next() >> (i + 1) * 2 % 255,
                        rng.next() >> 8 * (i + 1) % 255,
                        rng.next() % (50 * (i + 1)));
    };
    cv::Mat dst_(src_.size(), CV_8UC3);
    for (int i = 1; i <= stats.rows; i++) {
      cv::Mat mask = labels == i;
      auto color = random_color(i);
      dst_.setTo(color, mask);
    }
    return dst_;
  };

  threshed = get_components_image(std::move(threshed));
  cv::imshow(winname, threshed);
  cv::waitKey(0);
}