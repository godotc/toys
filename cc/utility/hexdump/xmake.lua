add_rules("mode.debug", "mode.release")

--add_requires("range-v3")
add_requires("cxxopts")

set_targetdir("bin")

set_languages("c++latest")
target("hexdump")
    set_kind("binary")
    add_files("src/*.cpp")

	add_packages("cxxopts")

