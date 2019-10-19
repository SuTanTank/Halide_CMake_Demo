cmake_minimum_required(VERSION 3.14)
project(halide_test_opencv)
find_package(OpenCV REQUIRED)
set(HALIDE_DISTRIB_DIR "d:/cpp/halide/" CACHE PATH "")
include(${HALIDE_DISTRIB_DIR}/halide.cmake)
add_library(Halide SHARED IMPORTED GLOBAL)
message("${HALIDE_DISTRIB_DIR}/Release/Halide.dll====")
set_target_properties(Halide PROPERTIES 
    INTERFACE_INCLUDE_DIRECTORIES ${HALIDE_DISTRIB_DIR}/include
    IMPORTED_LOCATION_RELEASE ${HALIDE_DISTRIB_DIR}/Release/Halide.dll
    IMPORTED_LOCATION_RELWITHDEBINFO ${HALIDE_DISTRIB_DIR}/Release/Halide.dll
    IMPORTED_LOCATION_DEBUG ${HALIDE_DISTRIB_DIR}/Debug/Halide.dll
    IMPORTED_IMPLIB_RELEASE ${HALIDE_DISTRIB_DIR}/Release/Halide.lib
    IMPORTED_IMPLIB_RELWITHDEBINFO ${HALIDE_DISTRIB_DIR}/Release/Halide.lib
    IMPORTED_IMPLIB_DEBUG ${HALIDE_DISTRIB_DIR}/Debug/Halide.lib
)

add_executable(halide_test_opencv test.cpp)
target_link_libraries(halide_test_opencv PUBLIC ${OpenCV_LIBS} Halide)
target_include_directories(halide_test_opencv PUBLIC ${HALIDE_INCLUDE_DIR} ${HALIDE_TOOLS_DIR})