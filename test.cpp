#include <cstdio>
#include <thread>
#include "Halide.h"
#include "clock.h"
#include "opencv2/opencv.hpp"

double tick(const char *name) {
    static double t_old = 0;
    double t_new = current_time();
    double dt = t_new - t_old;
    if (name) {
        printf("%s: %f\n", name, dt);
    }
    t_old = t_new;
    return dt;
}

int main() {
    using namespace Halide;

    auto target = get_host_target();
    target.set_feature(Target::Feature::SSE41);
    Var x, y, c;
    Func half_interleaved, half_planar;

    // interleaved
    auto img = cv::imread("007.jpg");
    auto input1 = Buffer<uint8_t>::make_interleaved(img.data, img.cols, img.rows, img.channels(), "input");
    half_interleaved(x, y, c) = cast<uint8_t>(cast<float>(input1(x, y, c)) * 0.5f);

    half_interleaved.output_buffer().dim(0).set_stride(img.channels());
    half_interleaved.output_buffer().dim(2).set_stride(1);

    half_interleaved.compile_jit(target);

    // planar
    auto input2 = Buffer<uint8_t>(img.data, img.cols * 3, img.rows, "input");
    half_planar(x, y) = cast<uint8_t>(cast<float>(input2(x, y)) * 0.5f);

    half_planar.compile_jit(target);

    // testing
    printf("TEST:\n");
    tick(NULL);
    // comparing to naive multi-threading
    auto pixel = img.cols * img.rows * 3;
    cv::Mat img_out2(img.size(), CV_8UC3);
    uchar *out_p = img_out2.data;
    uchar *in_p = img.data;
    std::vector<std::thread> threads(4);
    for (auto i = 0; i < 10000; ++i) {
        for (auto th = 0; th < threads.size(); ++th) {
            threads[th] = std::thread(
                [&](int start, int end) {
                    for (auto p = start; p < end; ++p) {
                        out_p[p] = in_p[p] * 0.5f + 0.5f;
                    }
                },
                th * pixel / threads.size(),
                (th + 1) * pixel / threads.size());
        }
        for (auto &th : threads)
            if (th.joinable())
                th.join();
    }
    tick("std");

    auto output1 = Buffer<uint8_t>::make_interleaved(img.cols, img.rows, img.channels(), "output");
    auto output2 = Buffer<uint8_t>(img.cols * 3, img.rows, "output");
    tick(NULL);
    for (int i = 0; i < 10000; ++i)
        half_interleaved.realize(output1);
    tick("half_interleaved");
    //
    tick(NULL);
    for (int i = 0; i < 10000; ++i)
        half_planar.realize(output2);
    tick("half_planar");

    // comparing to opencv
    cv::Mat img_f(img.size(), CV_32FC3);
    cv::Mat img_out(img.size(), CV_8UC3);
    for (int i = 0; i < 10000; ++i) {
        img.convertTo(img_f, CV_32FC3);
        img_f *= 0.5f;
        img_f.convertTo(img_out, CV_8UC3);
    }
    tick("OpenCV");

    printf("finished.\n");
    cv::Mat out_mat(img.rows, img.cols, img.type(), output2.data());
    cv::imshow("out2", out_mat);
    cv::waitKey();
    return 0;
}