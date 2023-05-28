add_rules("mode.debug", "mode.release")

add_requires("glut")

set_targetdir("bin")
add_includedirs("include")
add_packages("glut")


target("1")
    set_kind("binary")
    add_files("src/1_window.cpp")

target("2")
    set_kind("binary")
    add_files("src/2_block_anim.cpp")

target("3")
    set_kind("binary")
    add_files("src/3_pt_&_line.cpp")



