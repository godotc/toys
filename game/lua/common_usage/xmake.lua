add_rules("mode.debug", "mode.release")

add_requires("lua")
add_requires("gtest")

target("l")
do
	set_kind("binary")
	add_files("src/*.cpp")
	add_packages("lua")
	set_rundir(os.scriptdir())
end

includes("./xmake/tasks.lua")


target("t")
do
	set_kind("binary")
	add_files("src/test/*.cpp")
	add_packages("lua")
	add_packages("gtest")
	set_rundir(os.projectdir())
end

target("mylib")
do
	set_kind("shared")
	add_files("src/mylib/*.cpp")
	add_packages("lua")
end
