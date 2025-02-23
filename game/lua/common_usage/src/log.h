#pragma once

#include <cstdarg>
#include <cstdio>
#include <stdexcept>
extern "C" {
#include <lua.h>
}


inline void stack_dump(lua_State *L)
{

    int n = lua_gettop(L);
    for (int i = 1; i <= n; ++i) {
        int type = lua_type(L, i);
        switch (type) {
        case LUA_TSTRING:
            printf("'%s'\n", lua_tostring(L, i));
            break;
        case LUA_TNUMBER:
            printf("%g\n", lua_tonumber(L, i));
            break;
        case LUA_TBOOLEAN:
            printf("%s\n", lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TFUNCTION:
            printf("function\n");
            break;
        default:
            printf("%s\n", lua_typename(L, type));
        }
    }
}

inline void log(const char *fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    vprintf(fmt, vl);
    putchar('\n');
    va_end(vl);
}

inline void error(lua_State *L, const char *fmt, ...)
{

    va_list ap;
    va_list vl;
    va_start(vl, fmt);
    printf("error: ");
    vprintf(fmt, vl);
    putchar('\n');
    va_end(vl);
    stack_dump(L);
    throw std::runtime_error("lua error");
}


inline void error(const char *fmt, ...)
{

    va_list ap;
    va_list vl;
    va_start(vl, fmt);
    printf("error: ");
    vprintf(fmt, vl);
    putchar('\n');
    va_end(vl);
    throw std::runtime_error("lua error");
}
