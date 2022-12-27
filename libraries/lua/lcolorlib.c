#include <stdlib.h>
#include <stdio.h>

#define lcolorlib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#define COLOR "color"

typedef struct Color {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
} Color;

// Function to create a new color
static int color_new(lua_State *L) {
  unsigned char red = (unsigned char)luaL_optinteger(L, 1, 255);
  unsigned char green = (unsigned char)luaL_optinteger(L, 2, 255);
  unsigned char blue = (unsigned char)luaL_optinteger(L, 3, 255);
  unsigned char alpha = (unsigned char)luaL_optinteger(L, 4, 255);
  Color *color = (Color*)lua_newuserdata(L, sizeof(Color));
  color->red = red;
  color->green = green;
  color->blue = blue;
  color->alpha = alpha;
  luaL_getmetatable(L, COLOR);
  lua_setmetatable(L, -2);
  return 1;
}

// Function to get the red component of a color
static int color_get_red(lua_State *L) {
  Color *color = (Color*)luaL_checkudata(L, 1, COLOR);
  lua_pushinteger(L, color->red);
  return 1;
}

// Function to set the red component of a color
static int color_set_red(lua_State *L) {
  Color *color = (Color*)luaL_checkudata(L, 1, COLOR);
  color->red = (unsigned char)luaL_checkinteger(L, 2);
  return 0;
}

// Function to get the green component of a color
static int color_get_green(lua_State *L) {
  Color *color = (Color*)luaL_checkudata(L, 1, COLOR);
  lua_pushinteger(L, color->green);
  return 1;
}

// Function to set the green component of a color
static int color_set_green(lua_State *L) {
  Color *color = (Color*)luaL_checkudata(L, 1, COLOR);
  color->green = (unsigned char)luaL_checkinteger(L, 2);
  return 0;
}

// Function to get the blue component of a color
static int color_get_blue(lua_State *L) {
  Color *color = (Color*)luaL_checkudata(L, 1, COLOR);
  lua_pushinteger(L, color->blue);
  return 1;
}

// Function to set the blue component of a color
static int color_set_blue(lua_State *L) {
  Color *color = (Color*)luaL_checkudata(L, 1, COLOR);
  color->blue = (unsigned char)luaL_checkinteger(L, 2);
  return 0;
}

// Function to get the alpha component of a color
static int color_get_alpha(lua_State *L) {
  Color *color = (Color*)luaL_checkudata(L, 1, COLOR);
  lua_pushinteger(L, color->alpha);
  return 1;
}

// Function to set the alpha component of a color
static int color_set_alpha(lua_State *L) {
  Color *color = (Color*)luaL_checkudata(L, 1, COLOR);
  color->alpha = (unsigned char)luaL_checkinteger(L, 2);
  return 0;
}

static int color_tostring (lua_State *L)
{
  lua_pushstring(L, COLOR);
  return 1;
}

static const luaL_Reg color_meta[] = {
  {"__tostring", color_tostring},
  {"get_red", color_get_red},
  {"set_red", color_set_red},
  {"get_green", color_get_green},
  {"set_green", color_set_green},
  {"get_blue", color_get_blue},
  {"set_blue", color_set_blue},
  {"get_alpha", color_get_alpha},
  {"set_alpha", color_set_alpha},
  {NULL, NULL},
};


static void createmetatable (lua_State *L) {
  luaL_newmetatable(L, COLOR);  /* metatable for colors */
  luaL_register(L, NULL, color_meta);    /* fill metatable */
  lua_pushliteral(L, "__index");
  lua_pushvalue(L, -3);               /* dup methods table*/
  lua_rawset(L, -3);                  /* metatable.__index = methods */
  lua_pushliteral(L, "__metatable");
  lua_pushvalue(L, -3);               /* dup methods table*/
  lua_rawset(L, -3);                  /* hide metatable: metatable.__metatable = methods */
  lua_pop(L, 1);                      /* drop metatable */
  lua_pop(L, 1);  /* pop metatable */
}


static const luaL_Reg colorlib[] = {
  {"new", color_new},
  {NULL, NULL}
};


/*
** Open color library
*/
LUALIB_API int luaopen_color (lua_State *L) {
  luaL_register(L, LUA_COLORLIBNAME, colorlib);
  createmetatable(L);
  return 1;
}