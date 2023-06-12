add_rules("mode.debug", "mode.release")

add_requires("libsdl","glad")

set_languages("c++2a")
set_targetdir("bin")

includes("gltk")

add_includedirs("include","gltk")


target("0")
    set_kind("binary")
    add_files("src/triangle.cpp")
	add_packages("libsdl","glad")
	add_deps("resource_manager")

target("3bd")
    set_kind("binary")
    add_files("src/3body.cc")
	add_packages("libsdl","glad")
	add_deps("resource_manager")






