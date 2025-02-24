

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

// 练习29.1：请使用C语言编写一个可变长参数函数summation，来计算数值类型参数的和：
int Summation(lua_State *L)
{
    int narg = lua_gettop(L);

    printf("narg = %d\n", narg);
    int ret = 0;
    for (int i = 1; i <= narg; ++i) {
        int n = lua_tointeger(L, i);
        printf("arg %d\n", n);
        ret += n;
    }
    printf("ret %d\n", ret);
    lua_pop(L, narg);
    lua_pushinteger(L, ret);
    return 1;
}

static const struct luaL_Reg mylib[] = {
    {"summation", Summation},
    {    nullptr,   nullptr},
};


extern "C" {
int luaopen_mylib(lua_State *L)
{
    luaL_newlib(L, mylib);
    return 1;
}
}
