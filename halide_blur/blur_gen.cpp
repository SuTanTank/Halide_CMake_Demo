#include "Halide.h"
using namespace Halide;
class HalideBlur : public Generator<HalideBlur> {
public:
    Input<Buffer<uint8_t>> input{"input", 2};
    Output<Buffer<uint8_t>> result{"result", 2};

    void generate() {
        Func clamped = BoundaryConditions::repeat_edge(input);

        Func input_16("input_16");
        input_16(x, y) = cast<uint16_t>(clamped(x, y));

        blur_x(x, y) = (input_16(x - 1, y) + input_16(x, y) + input_16(x + 1, y)) / 3;

        blur_y(x, y) = (blur_x(x, y - 1) + blur_x(x, y) + blur_x(x, y + 1)) / 3;

        result(x, y) = cast<uint8_t>(blur_y(x, y));
    }

    void schedule() {}

private:
    Var x{"x"}, y{"y"};
    Func blur_x, blur_y;
};

HALIDE_REGISTER_GENERATOR(HalideBlur, HalideBlur);