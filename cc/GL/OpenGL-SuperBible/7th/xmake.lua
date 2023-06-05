add_rules("mode.debug", "mode.release")

add_requires("libsdl","glad")
add_packages("libsdl","glad")

set_languages("c++2a")
set_targetdir("bin")

add_includedirs("include")

target("0")
    set_kind("binary")
    add_files("src/main.cpp")

target("1")
    set_kind("binary")
    add_files("src/triangle.cpp")






