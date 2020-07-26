#include "blur.h"
#include "HalideBuffer.h"
using namespace Halide::Runtime;
void Blur(cv::Mat &img) {
    Buffer<uchar> buffer_in(img.data, img.cols, img.rows);
    cv::Mat out(img.size(), img.type());
    Buffer<uchar> buffer_out(out.data, out.cols, out.rows);
    HalideBlur(buffer_in, buffer_out);
    out.copyTo(img);
}
