cmake_minimum_required(VERSION 3.18)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/RELEASE)

enable_testing()



if(NOT CMAKE_BUILD_TOOL)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(H264Decoder)

add_subdirectory(test)

add_subdirectory(src)



