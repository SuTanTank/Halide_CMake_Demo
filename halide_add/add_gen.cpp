#include "Halide.h"
using namespace Halide;
class HalideAdd : public Generator<HalideAdd> {
public:
    Input<Buffer<uint8_t>> input{"input", 2};
    Output<Buffer<uint8_t>> result{"result", 2};

    void generate() { result(x, y) = clamp(input(x, y) + 20, 0, 255); }

    void schedule() {}

private:
    Var x{"x"}, y{"y"};
};

HALIDE_REGISTER_GENERATOR(HalideAdd, HalideAdd);