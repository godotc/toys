add_rules("mode.debug", "mode.release")


set_targetdir("bin")
set_languages("c++2a")

--add_defines('PROJECT_ROOT_DIR="$(projectdir)"')

add_includedirs("include")



-- Nonblock server
target("nblk")
	set_kind("binary")	
	add_files("test/prototype/non-block.cc")

--target("server")
--	add_files("src/*.cc")
