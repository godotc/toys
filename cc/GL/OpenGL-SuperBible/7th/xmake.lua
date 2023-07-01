
add_rules("mode.debug", "mode.release")

add_requires("glfw","glad","glm","fmt")
add_packages("glfw","glad","glm","fmt")
add_requires("gtest")

set_languages("c++20")
set_targetdir("bin")

includes("gltk")
includes("m_log")


if is_os("windows") then
	add_cxxflags("/Zc:preprocessor")
end


add_includedirs("include","gltk")
add_includedirs("m_log")


-- add_requires("libsdl")
-- target("0")
--     set_kind("binary")
--     add_files("src/triangle.cpp")
-- 	add_packages("libsdl","glad")
-- 	add_deps("gltk")
-- 	add_deps("m_log")

target("3bd")
    set_kind("binary")
    add_files("src/3body.cc")
	add_packages("libsdl","glad")
	add_deps("gltk")
	add_deps("m_log")




target("test")
	set_kind("binary")
	add_files("test/*.cc")
	add_packages("gtest")
	add_deps("m_log")
	

