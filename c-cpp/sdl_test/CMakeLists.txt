CMAKE_MINIMUM_REQUIRED(VERSION 3.2.0 )


project(hello)

add_executable(hello src/hello.cc)


find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(hello SDL2)
