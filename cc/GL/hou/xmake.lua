add_rules("mode.debug", "mode.release")

add_requires("glad", "glfw", "glm", "stb", "fmt")

add_packages("glad", "glfw", "glm", "stb", "fmt")

add_includedirs("pkg", "pkg/m_log")

includes("pkg/m_log")


target("m")
    set_kind("binary")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")

    add_deps("m_log")


