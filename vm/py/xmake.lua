add_rules("mode.debug", "mode.release")

set_languages("c++2a")
set_targetdir("bin")

add_requires("fmt")
add_packages("fmt")

includes("packages/m_log")
add_includedirs("packages/m_log")
-- Add this to top_level xmake if msvc
if is_os("windows") then
	add_cxxflags("/Zc:preprocessor")
end


add_includedirs("include")


target("py")
    set_kind("binary")
    add_files("src/**.cpp")
    add_files("src/**.cc")

	add_deps("m_log")


target("1")
	set_kind("binary")
	add_files("sample/1_arithmetic/arithmetic.cc")
	add_deps("m_log")



target("bfpt")
	set_kind("binary")
	add_files("test/binary_file_parse_test.cc")

	add_deps("m_log")


