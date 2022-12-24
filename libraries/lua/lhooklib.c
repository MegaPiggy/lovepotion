/*
** Library for Hooks
*/


#include <stddef.h>
#include <stdbool.h>

#define lhooklib_c
#define LUA_LIB

#include "lhooklib.h"

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

static void gethooks (lua_State *L) {
  lua_getfield(L, LUA_REGISTRYINDEX, "LUA_HOOKS");
  if (lua_isnil(L, -1)) {  /* no `hooks' table? */
    lua_pop(L, 1);  /* remove nil */
    lua_newtable(L);  /* create it */
    lua_pushvalue(L, -1);
    lua_setfield(L, LUA_REGISTRYINDEX, "LUA_HOOKS");  /* store in register */
  }
}

static void gethook (lua_State *L, lua_String event) {
  gethooks(L);
  lua_getfield(L, -1, event);
  if (lua_isnil(L, -1)) {  /* no event table? */
    lua_pop(L, 2);  /* remove nil and hooks */
    lua_newtable(L);  /* create it */
    gethooks(L);
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, event);  /* store in hooks */
    lua_pop(L, 1); /* remove hooks */
  }
}

void lua_addhook (lua_State *L, lua_String event, int funcindex, lua_String id) {
  int absidx = abs_index(L, funcindex);
  gethook(L, event);
  lua_pushvalue(L, absidx);  /* function */
  lua_setfield(L, -2, id); 
}

void lua_addhookc (lua_State *L, lua_String event, lua_CFunction function, lua_String id) {
  gethook(L, event);
  lua_pushcfunction(L, function);  /* function */
  lua_setfield(L, -2, id); 
}

int lua_callhook (lua_State *L, lua_String event, int idx, int n) {
  int absidx = abs_index(L, idx);
  gethook(L, event);
  int hookidx = abs_index(L, -1);
  lua_pushnil(L);  /* first key */
  while (lua_next(L, hookidx)) {
    lua_pushvalue(L, -1);  /* value */
    for (int i = 0; i < n; ++i) lua_pushvalue(L, absidx + i);
    lua_call(L, n, 0);
    lua_pop(L, 1);  /* remove value */
  }
  return 0;
}

static int hook_add (lua_State *L) {
  lua_String event = luaL_checkstring(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);
  lua_String id = luaL_checkstring(L, 3);
  lua_addhook(L, event, 2, id);
  return 0;
}
 
static int hook_call (lua_State *L) {
  lua_String event = luaL_checkstring(L, 1);
  lua_remove(L, 1);
  return lua_callhook(L, event, 1, lua_gettop(L));
}

static const luaL_Reg hook_funcs[] = {
  {"add", hook_add},
  {"call", hook_call},
  {NULL, NULL}
};

LUALIB_API int luaopen_hook (lua_State *L) {
  luaL_register(L, LUA_HOOKLIBNAME, hook_funcs);
  return 1;
}