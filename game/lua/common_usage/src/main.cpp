#include <any>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stack>
#include <string_view>
#include <variant>
#include <vector>
#include "log.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}



bool callLuafunc(lua_State *L, int nargs, int nret) {
  int ret = lua_pcall(L, nargs, nret, 0);
  if (ret != LUA_OK) {
    log("lua_pcall failed: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
    return false;
  }
  return true;
}

// sig: "dd>d"
std::vector<std::any> l_call(lua_State *L, const char *name,
                             std::string_view sig, ...) {

  va_list vl;

  va_start(vl, sig);

  lua_getglobal(L, name);

  int narg = sig.find('>');

  std::string_view retStr = sig.substr(narg + 1);

  luaL_checkstack(L, 1, "too many arguments");

  for (int i = 0; i < narg; ++i) {
    switch (sig[i]) {
    case 'd': {
      lua_pushnumber(L, va_arg(vl, double));
      break;
    }
    case 'i': {
      lua_pushinteger(L, va_arg(vl, int));
      break;
    }
    case 's': {
      lua_pushstring(L, va_arg(vl, char *));
      break;
    }
    case 'b': {
      lua_pushboolean(L, va_arg(vl, int));
      break;
    }
    default:
      error(L, "bad signature '%s', (%c)", sig, sig[i]);
    }
  }

  int nret = retStr.size();
  log("narg = %d, nret = %d\n", narg, nret);
  callLuafunc(L, narg, nret);

  std::vector<std::any> rets;
  for (int i = 0; i < nret; ++i) {
    char ch = retStr[i];
    // log(L, "ch = %c\n", ch);
    switch (ch) {
    case 'd': {
      int bNumber = -1;
      // -nret  + i
      //  nret = 2
      //  0  -2 +0 = -1
      //  1 -2 +1 = -1
      double v = lua_tonumberx(L, -nret + i, &bNumber);
      rets.push_back(v);
      break;
    }
    case 'i': {
      int bNumber = -1;
      int v = lua_tointegerx(L, -nret + i, &bNumber);
      rets.push_back(v);
      break;
    }
    case 'b': {
      bool v = lua_toboolean(L, -nret + i);
      rets.push_back(v);
      break;
    }
    case 's': {
      std::string v = lua_tostring(L, -nret + i);
      // log("s ret: %s\n", v.c_str());
      rets.push_back(v);
      break;
    }
    }
  }

  va_end(vl);
  return rets;
}

int main(int argc, char **argv) {

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  luaL_loadfile(L, "src/test.lua");
  callLuafunc(L, 0, 0);

  auto ret = l_call(L, "Test", "is>si", 1, "233");

  printf("ret = %s, %d\n", std::any_cast<std::string>(ret[0]).c_str(),
         std::any_cast<int>(ret[1]));

  return 0;
}
