
#pragma once
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}



inline bool callLuafunc(lua_State *L, int nargs, int nret)
{
    int ret = lua_pcall(L, nargs, nret, 0);
    if (ret != LUA_OK) {
        printf("lua_pcall failed: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}
