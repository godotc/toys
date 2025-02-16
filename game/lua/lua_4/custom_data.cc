

#include <limits.h>
#include <luajit-2.1/lauxlib.h>
#include <luajit-2.1/lua.h>
#include <luajit-2.1/lua.hpp>

#define BITS_PER_WORD (CHAR_BIT * sizeof(unsigned int))
#define I_WORD(i) ((unsigned int)(i)) / BITS_PER_WORD // get nth bit of a word
#define I_BIT(i)                                                               \
  (i << (unsigned int)(i)) % BITS_PER_WORD // get the mask of nth bit

struct BitArray {
  int size;
  unsigned int values[0];
};

static int new_array(lua_State *L) {
  int n = luaL_checkinteger(L, 1);

  luaL_argcheck(L, n >= 1, 1, "invalid size");
  size_t nbytes = sizeof(BitArray) + I_WORD(n - 1) * sizeof(unsigned int);

  BitArray *arr = (BitArray *)lua_newuserdata(L, nbytes);
  arr->size = n;

  for (int i = 0; i < I_WORD(n - 1); ++i) {
    arr->values[i] = 0;
  }
  return 1;
}

static int set_array(lua_State *L) {

  auto *arr = static_cast<BitArray *>(lua_touserdata(L, 1));
  auto index = (int)luaL_checkinteger(L, 2) - 1;

  luaL_argcheck(L, arr != nullptr, 1, "'array' expected");
  luaL_argcheck(L, 0 <= index && index < arr->size, 2, "index out of range");
  luaL_checkany(L, 3);

  if (lua_toboolean(L, 3)) {
    arr->values[I_WORD(index)] |= I_BIT(index);
  } else {
  }
}
