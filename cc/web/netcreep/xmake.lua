add_rules("mode.debug", "mode.release")

set_languages("c++20")
set_targetdir("bin")
set_symbols("debug")

add_requires("boost")


target("netcreep-server")
    set_kind("binary")
    add_files("src/server/*.cpp")

    add_packages("boost")

target("netcreep-client")
    set_kind("binary")
    add_files("src/client/*.cpp")

    add_packages("boost")


if is_plat("linux") then
    print("why linux?")
    add_cxflags("-Wall")
elseif  is_plat("windows") then
    add_defines("_MSVC_LANG=202002L")
    add_defines("NOMINMAX")
    add_cxflags("/Zc:preprocessor")
end



task("cpcm")
    set_menu{ description = "Regenerate the compile_commands" }
    on_run(function()
        import("script.utils")
        utils.execute_commands([[
            xmake f -c 
            xmake f -m  debug
            xmake project -k compile_commands
        ]])
    end)