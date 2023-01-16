
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <string.h>
#include <time.h>

static uint64_t time_monotonic() {
  struct timespec ti;
  clock_gettime(CLOCK_MONOTONIC, &ti);
  return (uint64_t)ti.tv_sec * 1000 + ti.tv_nsec / 1000000;
}

static uint64_t time_realtime() {
  struct timespec ti;
  clock_gettime(CLOCK_REALTIME, &ti);
  return (uint64_t)ti.tv_sec * 1000 + ti.tv_nsec / 1000000;
}

static uint64_t time_counter() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec*(uint64_t)(1000000000) + now.tv_nsec;
}

static uint64_t time_frequency() {
  return (uint64_t)(1000000000);
}

static int lmonotonic(lua_State* L) {
  lua_pushinteger(L, time_monotonic());
  return 1;
}

static int lrealtime(lua_State* L) {
  lua_pushinteger(L, time_realtime());
  return 1;
}

static int lcounter(lua_State* L) {
  uint64_t freq = lua_tointeger(L, lua_upvalueindex(1));
  uint64_t ti = time_counter();
  lua_pushnumber(L, (double)ti * 1000 / freq);
  return 1;
}

static const luaL_Reg time_funcs[] = {
  {"realtime", lrealtime},
  {"monotonic", lmonotonic},
  {"counter", NULL},
  {NULL, NULL}
};

LUALIB_API int luaopen_time (lua_State *L) {
  luaL_register(L, LUA_TIMELIBNAME, time_funcs);

  lua_pushinteger(L, time_frequency());
  lua_pushcclosure(L, lcounter, 1);
  lua_setfield(L, -2, "counter");

  return 1;
}