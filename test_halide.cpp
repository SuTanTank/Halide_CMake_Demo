#include "halide_add/add.h"
#include "halide_blur/blur.h"
#include "opencv2/opencv.hpp"
int main() {
    auto image = cv::imread("gray.png", cv::IMREAD_GRAYSCALE);
    Add(image);
    Blur(image);
    cv::imshow("blured", image);
    cv::waitKey();
    return 0;
}