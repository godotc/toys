add_rules("mode.debug", "mode.release")


add_requires("vulkan-hpp")

target("vulkan")
    set_kind("binary")
    add_files("src/*.cpp")
	add_packages("vulkan-hpp")

