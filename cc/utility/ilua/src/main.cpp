#include <iostream>


extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <cstdio>

thread_local int s_LuaRetVal;

void StackDump(lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {
        int t = lua_type(L, i);
        printf("%d: ", i);
        switch (t) {
        case LUA_TSTRING:
            printf("%s", lua_tostring(L, i));
            break;
        case LUA_TNUMBER:
            printf("%g", lua_tonumber(L, i));
            break;
        case LUA_TBOOLEAN:
            printf("%s", lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TFUNCTION:
            printf("function");
            break;
        case LUA_TTABLE:
            printf("table");
            break;
        case LUA_TUSERDATA:
            printf("userdata");
            break;
        case LUA_TTHREAD:
            printf("thread");
            break;
        default:
            printf("unknown type %s", lua_typename(L, t));
        }
    }
}

#define LUA_CALL(x) (s_LuaRetVal = (x),                                                                 \
                     (s_LuaRetVal != LUA_OK) ? printf("Error: %s | %s\n", #x, lua_tostring(L, -1)) : 0, \
                     s_LuaRetVal);



int lua_HostFunction(lua_State *L)
{
    float a = lua_tonumber(L, 1);
    float b = lua_tonumber(L, 2);
    lua_pushnumber(L, a + b);

    return 1;
}


int main(int argc, char **argv)
{

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);


    // LUA_CALL(luaL_loadfile(L, "test.lua"));

    printf("wtf\n");


    LUA_CALL(luaL_loadfile(L, "src/test.lua"));
    LUA_CALL(lua_pcall(L, 0, 0, 0));
    lua_getglobal(L, "a");
    int a = lua_tointeger(L, -1);

    printf("%d\n", a);

    if (LUA_TTABLE == lua_getglobal(L, "player")) {
        lua_pushstring(L, "name");
        lua_gettable(L, -2);
        auto name = lua_tostring(L, -1);
        printf("%s\n", name);
        lua_pop(L, 1);

        lua_pushstring(L, "age");
        lua_gettable(L, -2);
        auto age = lua_tostring(L, -1);
        printf("%s\n", age);
        lua_pop(L, 1);
    }
    else {
        printf("error not a table");
    }

    lua_register(L, "HostFunction", lua_HostFunction);
    lua_getglobal(L, "CallHostFunction");
    lua_pushnumber(L, 1);
    lua_pushnumber(L, 2);
    lua_pcall(L, 2, 0, 0);


    lua_close(L);

    return 0;
}
