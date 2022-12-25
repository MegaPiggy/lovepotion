#include "lua.h"

void lua_addhook (lua_State *L, lua_String event, int funcindex, lua_String id);
void lua_addhookc (lua_State *L, lua_String event, lua_CFunction function, lua_String id);
void lua_removehook (lua_State *L, lua_String event, lua_String id);
int lua_callhook (lua_State *L, lua_String event, int idx, int n);