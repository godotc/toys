
--add_requires("libsdl")
-- target("0")
--     set_kind("binary")
--     add_files("src/triangle.cpp")
-- 	add_packages("libsdl","glad")
-- 	add_deps("gltk")
-- 	add_deps("m_log")

target("3bd")
	set_kind("binary")
	add_files("3body.cc")
	add_packages("libsdl", "glad")
	add_deps("gltk")
	add_deps("m_log")




target("tri")
	set_kind("binary")
	add_files("triangle.cc")
	add_deps("gltk")
	add_deps("m_log")
