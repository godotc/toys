CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(GLFW_Test)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

add_executable(main)
set_property(TARGET main PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

aux_source_directory(src SRCS)
target_sources(main PUBLIC ${SRCS})


find_package(glfw3 CONFIG REQUIRED)
find_package(opengl REQUIRED)


target_link_libraries(main PRIVATE glfw opengl32)

message(>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   ${glfw3_DIR})
message(>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   ${CMAKE_INCLUDE_PATH})
message(>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   ${glew_PATH})
message(>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   ${opengl_PATH})

