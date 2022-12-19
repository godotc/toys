<<<<<<< HEAD
project(glog)

=======
cmake_minimum_required(VERSION 3.2.0)

project(glog)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SRCS CONFIGURE_DEPENDS ./src/*.cc)

add_library(glog ${SRCS})
target_include_directories(glog PUBLIC  ./include)

add_executable(test01 test/test.cc)
target_link_libraries(test01 PRIVATE glog)
target_include_directories(test01 PUBLIC ./include)

enable_testing()
add_test(NAME  TestingLog COMMAND test01)


>>>>>>> 811d362cba881050f3249f38b461f92207aad1ce


