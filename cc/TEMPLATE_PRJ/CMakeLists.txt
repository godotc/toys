cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)


if(NOT CMAKE_BUILD_TOOL)
    set(CMAKE_BUILD_TYPE Release)
endif()

enable_testing()
project(PROJECT)

add_subdirectory(test)
add_subdirectory(src)



