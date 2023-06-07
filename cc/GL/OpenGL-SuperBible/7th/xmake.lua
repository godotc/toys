add_rules("mode.debug", "mode.release")

add_requires("libsdl","glad")

set_languages("c++2a")
set_targetdir("bin")

includes("glutils")

add_includedirs("include","glutils")



target("0")
    set_kind("binary")
    add_files("src/main.cpp")
	add_packages("libsdl","glad")
	add_deps("resource_manager")

target("1")
    set_kind("binary")
    add_files("src/triangle.cpp")
	add_packages("libsdl","glad")
	add_deps("resource_manager")






