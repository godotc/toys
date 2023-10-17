add_rules("mode.debug", "mode.release")

set_targetdir("bin")

target("lexer")
    set_kind("binary")
    add_cxflags("-O0 -ggdb")
    add_files("src/*.cpp")
    set_toolchains("clang")

