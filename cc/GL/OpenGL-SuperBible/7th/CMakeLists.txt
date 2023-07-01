cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_TOOLCHAIN_FILE "E:/vcpkg/scripts/buildsystems/vcpkg.cmake")
#message(hello ${CMAKE_TOOLCHAIN_FILE})

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:preprocessor")
endif()

project(OPGL_SB_7th)

find_package(glfw3 REQUIRED)
find_package(glm CONFIG  REQUIRED)
find_package(glad  REQUIRED)
find_package(fmt CONFIG REQUIRED)

# find_package(SDL2 CONFIG REQUIRED)

add_subdirectory(m_log)
add_subdirectory(gltk)


add_executable(3bd)
target_sources(3bd PRIVATE src/3body.cc)
target_link_libraries(3bd PRIVATE 
    glfw
    opengl32
    glad::glad
    glm::glm
)
# target_link_libraries(3bd PRIVATE
#    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
#    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
# )
# target_link_libraries(3bd PRIVATE
#     SDL2::SDL2
# )
target_link_libraries(3bd PRIVATE 
    m_log
    gltk
)

include_directories(include)
include_directories(m_log)
include_directories(gltk)