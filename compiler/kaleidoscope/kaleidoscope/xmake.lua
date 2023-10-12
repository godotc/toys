add_rules("mode.debug", "mode.release")
set_languages("c++20")
set_policy("build.c++.modules", true)

target("cpl")
    set_kind("binary")
    add_files("src/**.cpp")

