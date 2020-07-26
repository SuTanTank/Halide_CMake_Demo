#include "add.h"
#include "HalideBuffer.h"
#include "opencv2/opencv.hpp"

using namespace Halide::Runtime;
void Add(cv::Mat &img) {
    Buffer<uchar> buffer_in(img.data, img.cols, img.rows);
    cv::Mat out(img.size(), img.type());
    Buffer<uchar> buffer_out(out.data, out.cols, out.rows);
    HalideAdd(buffer_in, buffer_out);
    out.copyTo(img);
}