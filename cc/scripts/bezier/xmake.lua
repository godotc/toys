
add_rules("mode.debug", "mode.release")
add_requires("opencv",{system=true})
--add_requires("qt5core", "qt5widgets")
--add_requires("glew")
--add_requires("fmt")
--
set_languages("c++latest")

target("bezier")
    set_kind("binary")
    add_files("src/*.cpp")
	add_packages("opencv")

	--add_packages("qt5core", "qt5widgets")
	--add_packages("glew")
	--add_packages("fmt")
