#include "Halide.h"

using namespace Halide;
int main() {
    Func out;
    Var x, y;
    out(x, y) = cast<uint8_t>(x) + cast<uint8_t>(y);
    Target target1 = get_host_target();
    Target target2 = get_host_target();
    target1.set_feature(Target::Feature::CUDA);
    out.compile_to_static_library("test", {}, "test_aot", target1);
    target2.set_feature(Target::Feature::NoRuntime);
    out.compile_to_static_library("test2", {}, "test_aot2", target2);
}