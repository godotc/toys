#include "clua.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>


LuaGlue LuaVersion(lua_State *L)
{
    puts("Console Program V0.1");
    puts(LUA_VERSION);
    puts(LUA_COPYRIGHT);
    puts(LUA_AUTHORS);
    // std::cout << LUA_AUTHORS;
    return 0;
}

// hook up LuaGlue functions
static std::vector<luaL_Reg> ConsoleGlue = {
    {"Version", LuaVersion},
 //{  nullptr,    nullptr}
};


const char *GetCommand()
{
    static char gpCommandBUffers[254];
    printf("-> ");
    std::cin >> gpCommandBUffers;
    return gpCommandBUffers;
    puts("\n");
}

int main(int argc, char **argv)
{
    using clua::cLua;

    puts("QUIT to quit");

    for (auto [name, func] : ConsoleGlue)
    {
        // printf("%s, %s\n", name, typeid(ConsoleGlue).name());
        cLua::Get()->AddFunction(name, func);
    }
    puts("load functions end...");

    std::string_view pCommand(GetCommand());
    while (pCommand != "QUIT")
    {
        puts(pCommand.begin());
        if (!cLua::Get()->RunString(pCommand)) {
            printf("ERROR:%s\n", cLua::Get()->GetErrorString().begin());
        }
        pCommand = GetCommand();
    }


    return 0;
}
