

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

    return 0;
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
