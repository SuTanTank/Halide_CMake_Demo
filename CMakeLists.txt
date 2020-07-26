cmake_minimum_required(VERSION 3.14)
project(halide_test_opencv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

find_package(OpenCV REQUIRED)
find_package(Halide REQUIRED)

include("add_sub_in_folder.cmake")
add_subdirectory_with_folder("HalideRuntime/" halide_rt)
add_subdirectory_with_folder("Blur/" halide_blur)
add_subdirectory_with_folder("Blur2/" halide_blur2)
add_executable(test_Blur test_blur.cpp)
target_link_libraries(test_Blur PUBLIC Blur ${OpenCV_LIBS})
