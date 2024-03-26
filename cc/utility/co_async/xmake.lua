add_rules("mode.debug", "mode.release")


set_targetdir("bin")

set_languages("c++latest")
target("co")
    set_kind("binary")
    add_files("src/*.cpp")


