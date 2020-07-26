#include "Halide.h"
using namespace Halide;
class HalideRuntime : public Generator<HalideRuntime> {
public:
    Input<Buffer<uint8_t>> input{"input", 2};
    Output<Buffer<uint8_t>> result{"result", 2};

    void generate() { result(x, y) = input(x, y); }

    void schedule() {}

private:
    Var x{"x"}, y{"y"};
};

HALIDE_REGISTER_GENERATOR(HalideRuntime, HalideRuntime);