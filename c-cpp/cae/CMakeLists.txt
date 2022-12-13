cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEBUG ON)
if(NOT DEBUG)
#if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#--------------------------------------------------------------------
add_subdirectory(ai-cae)