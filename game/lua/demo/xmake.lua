add_rules("mode.debug", "mode.release")

add_requires("lua")
add_requires("directxtk")

set_targetdir("bin")

add_includedirs("include")

target("console")
    set_kind("binary")
    add_files("src/console_program.cc")
    add_packages("lua")


target("sample")
    set_kind("binary")
    add_files("src/main.cc")
    add_packages("lua")
    add_packages("directxtk")