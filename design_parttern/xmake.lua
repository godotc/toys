add_rules("mode.debug", "mode.release")

add_requires("gtest")
add_requires("fmt")
add_packages("fmt")

-- Add this to top_level xmake if msvc
if is_os("windows") then
	add_cxxflags("/Zc:preprocessor")
end


set_targetdir("bin")
set_languages("c++2a")


includes("pkgs/m_log")
add_includedirs("pkgs/m_log")
-- Add this to top_level xmake if msvc
if is_os("windows") then
	add_cxxflags("/Zc:preprocessor")
end

add_includedirs("include")


target("_")
    set_kind("binary")
    add_files("src/**.cpp")
	add_includedirs("include")
	add_deps("m_log")


