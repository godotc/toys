# this is the build file for project 
# it is autogenerated by the xmake build system.
# do not edit by hand.

# project
cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0091 NEW)
project(py LANGUAGES CXX C)

# target
add_executable(py "")
set_target_properties(py PROPERTIES OUTPUT_NAME "py")
set_target_properties(py PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/linux/x86_64/release")
target_compile_options(py PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64>
    $<$<COMPILE_LANGUAGE:CXX>:-m64>
    $<$<COMPILE_LANGUAGE:C>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
)
if(MSVC)
    target_compile_options(py PRIVATE $<$<CONFIG:Release>:-Ox -fp:fast>)
else()
    target_compile_options(py PRIVATE -O3)
endif()
if(MSVC)
else()
    target_compile_options(py PRIVATE -fvisibility=hidden)
endif()
if(MSVC)
    set_property(TARGET py PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
target_link_options(py PRIVATE
    -m64
)
target_sources(py PRIVATE
    src/main.cpp
)

