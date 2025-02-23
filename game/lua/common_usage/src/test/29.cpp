
#include <gtest/gtest.h>

#include "../types.h"
#include <filesystem>

TEST(lua, 29_1)
{
    printf("cwd = %s\n", std::filesystem::current_path().string().c_str());

    printf("???\n");
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    printf("1\n");
    int ret = luaL_loadfile(L, "src/test/29.lua");
    ASSERT_EQ(ret, LUA_OK);

    printf("2\n");
    bool retb = callLuafunc(L, 0, 0);
    ASSERT_TRUE(retb);

    printf("3\n");
}
