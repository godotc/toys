
project(engine)

set(ENGINE engine)
file(GLOB ENGINE_SRCS CONFIGURE ./src/engine/*.cc ./src/engine/*.cpp)
add_library( ${ENGINE} ${ENGINE_SRCS})
target_include_directories(${ENGINE} PRIVATE include)