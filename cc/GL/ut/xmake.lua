add_rules("mode.debug", "mode.release")

add_requires("glut","glfw")

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

target("4")
    set_kind("binary")
    add_files("src/4_triangle_or_polygon.cpp")

target("5")
    set_kind("binary")
    add_files("src/5_solid.cc")

target("6")
    set_kind("binary")
    add_files("src/6_polygon_pts_stipple.cc")




target("glfw")
	set_kind("binary")
	add_files("test/glfw_test.cc")
	add_packages("glfw")


