add_rules("mode.debug", "mode.release")

add_requires("glfw","glm","stb")
add_requires("glad")

set_targetdir("bin")
set_languages("c++2a")

add_defines('PROJECT_ROOT_DIR="$(projectdir)"')

target("breakout")
    set_kind("binary")
    add_files("src/*.cpp")
	add_packages("glfw","glm","stb")
	add_packages("glad")


--target("t")
--    set_kind("binary")
--    add_files("src/main.cpp")
--	add_packages("glfw","glm","stb")
--	add_packages("glad")
--
--target("path_test")
--    set_kind("binary")
--    add_files("test/path.cc")
