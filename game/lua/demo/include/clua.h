#pragma once
#include <filesystem>
#include <functional>
#include <memory>
#include <string_view>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

namespace clua {

#define LuaGlue extern "C" int

extern "C" {
// using LuaFunctionType = std::function<int(lua_State *)>;
using LuaFunctionType = int (*)(struct lua_State *);
}

class cLua
{
  public:
    static auto Get()
    {
        static std::shared_ptr<cLua> Lua(new clua::cLua);
        return Lua;
    }

    cLua()
    {
        m_State = luaL_newstate();
        luaopen_base(m_State);
        luaopen_io(m_State);
        luaopen_string(m_State);
        luaopen_math(m_State);
        luaopen_debug(m_State);
    }
    virtual ~cLua()
    {
        lua_close(m_State);
    }

    bool RunScript(std::filesystem::path filename);
    bool RunString(std::string_view command)
    {
        // send command to the Lua env
        // if (luaL_loadstring(m_State, command.begin() + 1) == 0)
        if (luaL_loadbuffer(m_State, command.begin(), command.size(), nullptr) == 0)
        {
#if 1
            puts("success send to buffer");
#endif
            if (!lua_pcall(m_State, 0, LUA_MULTRET, 0))
            {
                return true;
            }
        }
        return false;
    }

    bool AddFunction(std::string_view name, LuaFunctionType func)
    {
        lua_register(m_State, name.begin(), func);

        return true;
    }

    std::string_view GetErrorString() { return luaL_checkstring(m_State, -1); }

    std::string_view GetStringArgument(int num, const char *pDefault = nullptr);
    std::string_view GetNumerArgument(int num, double pDefault = 0.0);
    void             PushString(const char *string);
    void             PushNumber(double value);

  private:
    lua_State *m_State;
};

} // namespace clua