add_requires("lua")
add_rules("mode.debug", "mode.release")


target("ilua")
do
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("lua")
    set_rundir("$(projectdir)")
end


