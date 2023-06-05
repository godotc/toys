add_rules("mode.debug", "mode.release")

add_requires("libsdl")

target("sdl_soft_render")
    set_kind("binary")
    add_files("src/*.cpp")
	add_packages("libsdl")

