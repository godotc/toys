add_requires("glad","glfw","glm","stb")


target("gltk")
	set_kind("shared")

	if is_os("linux") then
		add_cxflags("-fPIC")
	elseif is_os("windows") then
		add_defines("BUILD_SHARED_GLTK")
	end

	add_files("./resource_manager/*.cpp")
	add_packages("glad","glfw","glm","stb")

	add_deps("m_log")

	add_packages("fmt")


