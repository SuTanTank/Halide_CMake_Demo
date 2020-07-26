cmake_minimum_required(VERSION 3.14)
project(halide_test_opencv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

find_package(OpenCV REQUIRED)
find_package(Halide REQUIRED)

include("add_sub_in_folder.cmake")
add_subdirectory_with_folder("HalideRuntime/" halide_rt)
add_subdirectory_with_folder("Add/" halide_add)
add_subdirectory_with_folder("Blur/" halide_blur)
add_executable(HalideDemo test_halide.cpp)
target_link_libraries(HalideDemo PUBLIC Add Blur ${OpenCV_LIBS})
