add_rules("mode.debug", "mode.release")


set_targetdir("bin")
set_symbols("debug")

set_languages("c++23")
target("co")
    set_kind("binary")
    add_files("src/*.cpp")


