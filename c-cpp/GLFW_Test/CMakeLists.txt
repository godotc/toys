CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(GLFW_Test)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

add_executable(main)
set_property(TARGET main PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SRCS CONFIGURE_DEPENDS src/*.cc)
#aux_source_directory(src SRCS)
target_sources(main PUBLIC ${SRCS})


find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw )


if(WIN32)
	message(STATUS >>>> ENV:WIN32 --- finding opengl32)
	find_package(opengl REQUIRED)
	target_link_libraries(main PRIVATE opengl32)
else()
	message(STATUS >>>> ENV:LINUX --- finding OpenGL/GL)
	find_package(OpenGL)
	target_link_libraries(main PRIVATE OpenGL)
endif()



message(>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   ${CMAKE_INCLUDE_PATH})
message(>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   ${glfw3_DIR})

