add_executable(add.gen add_gen.cpp)
target_link_libraries(add.gen PRIVATE Halide::Generator)
add_halide_library(HalideAdd FROM add.gen
    USE_RUNTIME HalideRuntime)
add_library(Add STATIC add.cpp add.h)
target_link_libraries(Add PUBLIC HalideAdd ${OpenCV_LIBS})