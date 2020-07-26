add_executable(blur.gen blur_gen.cpp)
target_link_libraries(blur.gen PRIVATE Halide::Generator)
add_halide_library(HalideBlur FROM blur.gen
    USE_RUNTIME HalideRuntime)
add_library(Blur STATIC blur.cpp blur.h)
target_link_libraries(Blur PUBLIC HalideBlur ${OpenCV_LIBS})