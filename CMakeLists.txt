cmake_minimum_required(VERSION 3.14)
project(halide_test_opencv)
find_package(OpenCV REQUIRED)
set(HALIDE_DISTRIB_DIR "d:/cpp/halide/" CACHE PATH "path to halide distribution")
add_library(Halide SHARED IMPORTED GLOBAL)
if (WIN32)
  message(STATUS "Using Shared Library: ${HALIDE_DISTRIB_DIR}/$<CONFIG>/${CMAKE_SHARED_LIBRARY_PREFIX}Halide${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set_target_properties(Halide PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${HALIDE_DISTRIB_DIR}/include
    IMPORTED_LOCATION_RELEASE ${HALIDE_DISTRIB_DIR}/Release/${CMAKE_SHARED_LIBRARY_PREFIX}Halide${CMAKE_SHARED_LIBRARY_SUFFIX}
    IMPORTED_LOCATION_RELWITHDEBINFO ${HALIDE_DISTRIB_DIR}/Debug/${CMAKE_SHARED_LIBRARY_PREFIX}Halide${CMAKE_SHARED_LIBRARY_SUFFIX}
    IMPORTED_LOCATION_DEBUG ${HALIDE_DISTRIB_DIR}/Debug/${CMAKE_SHARED_LIBRARY_PREFIX}Halide${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  set_target_properties(Halide PROPERTIES
    IMPORTED_IMPLIB_RELEASE ${HALIDE_DISTRIB_DIR}/Release/${CMAKE_STATIC_LIBRARY_PREFIX}Halide${CMAKE_STATIC_LIBRARY_SUFFIX}
    IMPORTED_IMPLIB_RELWITHDEBINFO ${HALIDE_DISTRIB_DIR}/Debug/${CMAKE_STATIC_LIBRARY_PREFIX}Halide${CMAKE_STATIC_LIBRARY_SUFFIX}
    IMPORTED_IMPLIB_DEBUG ${HALIDE_DISTRIB_DIR}/Debug/${CMAKE_STATIC_LIBRARY_PREFIX}Halide${CMAKE_STATIC_LIBRARY_SUFFIX}
  )
else()
  message(STATUS "Using Shared Library: ${HALIDE_DISTRIB_DIR}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}Halide${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set_target_properties(Halide PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${HALIDE_DISTRIB_DIR}/include
    IMPORTED_LOCATION ${HALIDE_DISTRIB_DIR}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}Halide${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
endif()
target_include_directories(Halide INTERFACE ${HALIDE_DISTRIB_DIR}/tools)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rgb.png ${CMAKE_CURRENT_BINARY_DIR}/rgb.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gray.png ${CMAKE_CURRENT_BINARY_DIR}/gray.png COPYONLY)

add_executable(halide_test_opencv test.cpp)
target_link_libraries(halide_test_opencv PUBLIC ${OpenCV_LIBS} Halide)
target_include_directories(halide_test_opencv PUBLIC)

add_executable(generator generator.cpp)
target_link_libraries(generator PRIVATE Halide)
target_include_directories(generator PRIVATE)

set(output_dir ${CMAKE_CURRENT_BINARY_DIR})
set(RUN_WITHOUT_LEAKCHECK ${CMAKE_COMMAND} -E env "ASAN_OPTIONS=detect_leaks=0")
set (COMBINED_LIB "halide_lib${CMAKE_STATIC_LIBRARY_SUFFIX}")
if (WIN32)
add_custom_command(OUTPUT ${COMBINED_LIB}
                    DEPENDS generator
                    COMMAND ${RUN_WITHOUT_LEAKCHECK} $<TARGET_FILE:generator>
                    COMMAND lib.exe /out:halide_lib.lib *.lib
                    WORKING_DIRECTORY "${output_dir}"
                    COMMENT "-=-=-=-=-=-=-=-=-=-=       generating stuff in ${output_dir}      -= -==-=-=--=-===" )
elseif (ANDROID)

elseif (APPLE AND NOT IOS)

else()

endif()

add_custom_target(exec_generate
                    DEPENDS ${COMBINED_LIB})
add_executable(generator_test
                 generator_test.cpp)
add_dependencies(generator_test
                   exec_generate)

add_library(custom_lib STATIC IMPORTED)
set_target_properties(custom_lib PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${output_dir}
  IMPORTED_LOCATION ${output_dir}/${COMBINED_LIB}
)
target_link_libraries(generator_test PRIVATE ${OpenCV_LIBS})
target_link_libraries(generator_test PRIVATE custom_lib)
target_include_directories(generator_test PRIVATE ${output_dir})

target_include_directories(generator_test PRIVATE "${HALIDE_DISTRIB_DIR}/include")