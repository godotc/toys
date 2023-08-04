add_rules("mode.debug", "mode.release")

set_languages("c++20")
-- add_cxflags("-stdlib=libc++")


target("cpplings")
    set_kind("binary")
    add_files("src/*.cpp")
    -- add_files("src/*.cppm")



target("manual_test")
    add_files("test/manual/*.cc")
    add_includedirs("src")


