add_rules("mode.debug", "mode.release")

set_languages("c++2a")
set_targetdir("bin")

target("py")
    set_kind("binary")
    add_files("src/*.cpp")



target("1")
	set_kind("binary")
	add_files("sample/1_arithmetic/arithmetic.cc")

