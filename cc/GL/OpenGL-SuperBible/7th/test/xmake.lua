
target("test")
	set_kind("binary")
	add_files("*.cc")
	add_packages("gtest")
	add_deps("m_log")
