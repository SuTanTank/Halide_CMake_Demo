#include "test.h"
#include "test2.h"

#include "HalideBuffer.h"
#include "opencv2/opencv.hpp"
int main() {
    Halide::Runtime::Buffer<uint8_t> output(640, 480);

    int error1 = test_aot(output);
    int error2 = test_aot2(output);
    cv::Mat mat(480, 640, CV_8UC1, output.data());
    cv::imshow("mat", mat);
    cv::waitKey();
    return error1;
}