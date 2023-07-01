cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0091 NEW)
project(gltk LANGUAGES CXX C)

ADD_DEFINITIONS(-DBUILD_SHARED_GLTK)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(gltk SHARED "")

# Set target properties
set_target_properties(gltk PROPERTIES OUTPUT_NAME "gltk")
target_compile_definitions(gltk PRIVATE GLFW_INCLUDE_NONE)
# set_target_properties(gltk PROPERTIES 
#     POSITION_INDEPENDENT_CODE ON
#     WINDOWS_EXPORT_ALL_SYMBOLS ON
# )

# Link libraries
target_link_libraries(gltk PRIVATE
    glfw
    opengl32
    glad::glad
    glm::glm
)

# Add include directories
target_include_directories(gltk PRIVATE .)
target_include_directories(gltk PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(gltk PRIVATE ${CMAKE_SOURCE_DIR}/m_log)

# Add source files
target_sources(gltk PRIVATE
    resource_manager/shader.cpp
    resource_manager/texture.cpp
    resource_manager/resource_manager.cpp
)



target_link_libraries(gltk PUBLIC
    m_log
)