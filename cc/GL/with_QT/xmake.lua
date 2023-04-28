add_rules("mode.debug", "mode.release")

set_languages("c++2a")
add_requires("glfw","glew")

target("with_QT")
    add_rules("qt.widgetapp")
    add_headerfiles("src/*.h")
    add_files("src/*.cpp")
    add_files("src/mainwindow.ui")
	add_files("shader.qrc")
    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files("src/mainwindow.h")
	add_packages("glfw", "glew")


