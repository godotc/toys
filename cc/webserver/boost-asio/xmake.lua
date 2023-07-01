add_rules("mode.debug", "mode.release")

set_targetdir("bin")
set_languages("c++2a")

add_requires("boost")
add_packages("boost")
add_defines("_WIN32_WINDOWS")

add_requires("fmt")
add_packages("fmt")

includes("m_log")

if is_os("windows") then
	add_cxxflags("/Zc:preprocessor")
end

add_includedirs("include")
add_includedirs("include","m_log")
add_files("src/boost_pch.cpp")

target("boost-asio")
	set_kind("binary")
	add_files("src/*.cpp")
	add_deps("m_log")

target("1_1_sync_server")
	set_kind("binary")
	add_files("sample/3_synchornous_server/server.cpp")
	add_deps("m_log")

target("1_2_sync_client")
	set_kind("binary")
	add_files("sample/3_synchornous_server/client.cpp")
	add_deps("m_log")


