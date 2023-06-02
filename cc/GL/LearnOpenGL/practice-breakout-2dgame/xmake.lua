add_rules("mode.debug", "mode.release")

add_requires("glfw","glm","stb")
add_requires("glad")
add_requires("gtest")

set_targetdir("bin")
set_languages("c++2a")

add_defines('PROJECT_ROOT_DIR="$(projectdir)"')

target("breakout")
    set_kind("binary")
    add_files("src/**.cpp")
	add_includedirs("include")
	add_packages("glfw","glm","stb")
	add_packages("glad")


target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_files("test/*.cc")
	add_includedirs("src","include")
	add_defines("DISABLE_TEST_CASE")
	add_packages("glfw","glm","stb")
	add_packages("glad")
	add_packages("gtest")
