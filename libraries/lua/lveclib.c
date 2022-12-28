#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#define lveclib_c
#define LUA_LIB

#include "lua.h"

#include "lapi.h"
#include "lauxlib.h"
#include "ldebug.h"
#include "lgc.h"
#include "llimits.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "lualib.h"
#include "lvm.h"

static int vector_new(lua_State* L)
{
    lua_Number x = luaL_checknumber(L, 1);
    lua_Number y = luaL_checknumber(L, 2);
    lua_Number z = luaL_checknumber(L, 3);

#if LUA_VECTOR_SIZE == 4
    lua_Number w = luaL_optnumber(L, 4, 0.0);
    lua_pushvector(L, (float)(x), (float)(y), (float)(z), (float)(w));
#else
    lua_pushvector(L, (float)(x), (float)(y), (float)(z));
#endif
    return 1;
}

static int vector_dot(lua_State* L)
{
    lua_Vector a = luaL_checkvector(L, 1);
    lua_Vector b = luaL_checkvector(L, 2);

    lua_pushnumber(L, a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
    return 1;
}

static int vector_magnitude(lua_State* L)
{
    lua_Vector v = luaL_checkvector(L, 1);

    lua_pushnumber(L, sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
    return 1;
}


static const luaL_Reg vectorlib[] = {
  {"new", vector_new},
  {"dot", vector_dot},
  {"magnitude", vector_magnitude},
  {NULL, NULL}
};


static void createmetatable (lua_State *L) {
  luaL_newmetatable(L, "vector");  /* create metatable for vectors */
  /* dummy vector */
#if LUA_VECTOR_SIZE == 4
    lua_pushvector(L, 0.0f, 0.0f, 0.0f, 0.0f);
#else
    lua_pushvector(L, 0.0f, 0.0f, 0.0f);
#endif
  lua_pushvalue(L, -2);
  lua_setmetatable(L, -2);  /* set vector metatable */
  lua_pop(L, 1);  /* pop dummy vector */
  lua_pushvalue(L, -2);  /* vector library... */
  lua_setfield(L, -2, "__index");  /* ...is the __index metamethod */
  lua_setreadonly(L, -1, true);
  lua_pop(L, 1);  /* pop metatable */
}


/*
** Open vector library
*/
LUALIB_API int luaopen_vector (lua_State *L) {
  luaL_register(L, LUA_VECTORLIBNAME, vectorlib);
  createmetatable(L);
  return 1;
}