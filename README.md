# Halide OpenCV and CMake Demo
This project shows how to use Halide's new CMake integration, along with a CMake demo on how to use Halide with an ordinary image processing library which is using OpenCV ```Mat``` as image data type. 

OpenCV and Halide are find by ```find_package()```. For Windows, usually you should set ```OpenCV_DIR``` and ```Halide_DIR``` manually.

A stand-alone ```HalideRuntime``` library is created to make sure all other filters are using this runtime to avoid linking issue on Windows.

Two filters ```Add``` and ```Blur``` are defined, which shows how multiple filters are created in a large CMake project. 