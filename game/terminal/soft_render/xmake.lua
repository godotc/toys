add_rules("mode.debug", "mode.release")

set_targetdir("bin")
add_requires("ncurses")

add_includedirs("include")
add_cxflags("-O0")

target("main")
	set_kind("binary")
	add_files("src/*.cc")
	add_cxflags("-lncurses")
	add_packages("glm","ncurses")
	
