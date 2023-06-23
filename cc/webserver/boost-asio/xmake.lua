add_rules("mode.debug", "mode.release")

set_targetdir("bin")
set_languages("c++2a")

--add_requires("boost")
--add_packages("boost")

add_includedirs("include")
add_files("src/boost_pch.cpp")

target("boost-asio")
	set_kind("binary")
	add_files("src/*.cpp")

target("1_1_sync_server")
	set_kind("binary")
	add_files("sample/3_synchornous_server/server.cpp")

target("1_2_sync_client")
	set_kind("binary")
	add_files("sample/3_synchornous_server/client.cpp")


