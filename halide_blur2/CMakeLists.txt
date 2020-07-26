add_executable(blur2.gen blur_gen.cpp)
target_link_libraries(blur2.gen PRIVATE Halide::Generator)
add_halide_library(HalideBlur2 FROM blur2.gen
    USE_RUNTIME HalideRuntime)
add_library(Blur2 STATIC blur.cpp blur.h)
target_link_libraries(Blur2 PUBLIC HalideBlur2 ${OpenCV_LIBS})