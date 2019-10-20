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
    auto target = get_target_from_environment();
    //target.set_feature(Target::Feature::AVX2);

    // read image to cv::Mat
    auto img = cv::imread("007.jpg");
    if (img.empty())
        return 1;

    Var x("x"), y("y"), c("c");
    Var xo, yo, xi, yi, xytile;
    Var block, thread;

    // interleaved
    Func half_interleaved("interleaved");
    auto input1 = Buffer<uint8_t>::make_interleaved(img.data, img.cols, img.rows, img.channels(), "input");
    half_interleaved(x, y, c) = cast<uint8_t>(cast<float>(input1(x, y, c)) * 0.5f);

    half_interleaved.output_buffer().dim(0).set_stride(img.channels());
    half_interleaved.output_buffer().dim(2).set_stride(1);
    // Var xc("xc");
    half_interleaved.parallel(y);

    half_interleaved.print_loop_nest();
    half_interleaved.compile_jit(target);

    // planar
    Func half_planar("planar");
    auto input2 = Buffer<uint8_t>(img.data, img.cols * 3, img.rows, "input");
    half_planar(x, y) = cast<uint8_t>(cast<float>(input2(x, y)) * 0.5f);
    half_planar.parallel(y);
    half_planar.print_loop_nest();
    half_planar.compile_jit(target);
    // testing
    printf("TEST:\n");
    cv::namedWindow("out", 0);
    // comparing to naive multi-threading
    {
        auto pixel = img.cols * img.rows * 3;
        cv::Mat out_mat(img.size(), img.type());
        uchar *out_p = out_mat.data;
        uchar *in_p = img.data;
        std::vector<std::thread> threads(8);
        tick(NULL);
        for (auto i = 0; i < 1000; ++i) {
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
        cv::imshow("out", out_mat);
        cv::waitKey(100);
    }
    {
        auto output1 = Buffer<uint8_t>::make_interleaved(img.cols, img.rows, img.channels(), "output");
        half_interleaved.realize(output1);
        tick(NULL);
        for (int i = 0; i < 1000; ++i)
            half_interleaved.realize(output1);
        tick("half_interleaved");
        cv::Mat out_mat(img.rows, img.cols, img.type(), output1.data());
        cv::imshow("out", out_mat);
        cv::waitKey(100);
    }
    {
        auto output2 = Buffer<uint8_t>(img.cols * 3, img.rows, "output");
        half_planar.realize(output2);
        tick(NULL);
        for (int i = 0; i < 1000; ++i)
            half_planar.realize(output2);
        tick("half_planar");
        cv::Mat out_mat(img.rows, img.cols, img.type(), output2.data());
        cv::imshow("out", out_mat);
        cv::waitKey(100);
    }
    {
        // comparing to opencv
        cv::Mat img_f(img.size(), CV_32FC3);
        cv::Mat out_mat(img.size(), CV_8UC3);
        img.convertTo(img_f, CV_32FC3);
        img_f *= 0.5f;
        img_f.convertTo(out_mat, CV_8UC3);
        tick(NULL);
        for (int i = 0; i < 1000; ++i) {
            img.convertTo(img_f, CV_32FC3);
            img_f *= 0.5f;
            img_f.convertTo(out_mat, CV_8UC3);
        }
        tick("OpenCV");
        cv::imshow("out", out_mat);
        cv::waitKey(100);
    }
    printf("finished.\n");
    
    return 0;
}