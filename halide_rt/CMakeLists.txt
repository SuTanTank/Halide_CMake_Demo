add_executable(rt.gen rt_gen.cpp)
target_link_libraries(rt.gen PRIVATE Halide::Generator) 
add_halide_library(HalideRuntime FROM rt.gen)
set_target_properties(HalideRuntime PROPERTIES IMPORTED_GLOBAL TRUE)