add_rules("mode.debug", "mode.release")

add_packages(
	"glfw", "glad", "libsdl",
	"glm", "fmt", "gtest",
	"stb")
add_requires(
	"glfw", "glad", "libsdl",
	"glm", "fmt", "gtest",
	"stb")


set_languages("c++20")
set_targetdir("bin")

includes("pkgs/gltk","pkgs/m_log")
includes("src","test")


if is_os("windows") then
	add_cxxflags("/Zc:preprocessor")
end


add_includedirs("include")
add_includedirs("pkgs/m_log", "pkgs/gltk")


