add_rules("mode.debug", "mode.release")

add_requires("glfw","glm","stb")
add_requires("glad")
add_requires("gtest")
add_requires("fmt")

add_packages("glfw","glm","stb","glad","fmt")

set_targetdir("bin")
set_languages("c++2a")

add_defines('PROJECT_ROOT_DIR="$(projectdir)"')

includes("m_log")
add_includedirs("m_log")
-- Add this to top_level xmake if msvc
if is_os("windows") then
	add_cxxflags("/Zc:preprocessor")
end

target("breakout")
    set_kind("binary")
    add_files("src/**.cpp")
	add_includedirs("include")
	add_packages("glfw","glm","stb")
	add_packages("glad")
	add_packages("fmt")
	add_deps("m_log")


target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_files("test/*.cc")
	add_includedirs("src","include")

	add_defines("DISABLE_TEST_CASE")

	add_packages("glfw","glm","stb")
	add_packages("glad")
	add_packages("gtest")

	add_deps("m_log")
