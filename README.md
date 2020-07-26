# Halide CMake Template, work with OpenCV
This project gives a simple CMake template, which demostrates how to use Halide with an ordinary image processing library which is using OpenCV ```Mat``` as image data type. 

OpenCV and Halide are find via ```find_package()```. For Windows, usually you should set ```OpenCV_DIR``` and ```Halide_DIR``` manually.

A stand-alone ```HalideRuntime``` library is created to make sure all other filters are using this runtime to avoid linking issue on Windows. This halide library should have ```IMPORT_GLOBAL``` property such that other halide libraries can find it.

Two filters ```Add``` and ```Blur``` are defined, which shows how multiple filters are created and co-exist in a large CMake project. 