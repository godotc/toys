add_rules("mode.debug", "mode.release")
add_requires("lua")

target("l")
do
	set_kind("binary")
	add_files("src/*.cpp")
	add_packages("lua")
	set_rundir(os.scriptdir())
end
