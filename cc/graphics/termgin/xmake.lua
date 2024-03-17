add_rules("mode.debug", "mode.release")

target("termgin")
    set_kind("binary")
    add_files("src/main.cpp")
