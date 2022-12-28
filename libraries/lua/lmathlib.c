/*
** $Id: lmathlib.c,v 1.67.1.1 2007/12/27 13:02:25 roberto Exp $
** Standard mathematical library
** See Copyright Notice in lua.h
*/


#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define lmathlib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include "llimits.h"


#undef LUA_MAXINTEGER
#define LUA_MAXINTEGER (INT_MAX)
#undef LUA_MININTEGER
#define LUA_MININTEGER (INT_MIN)
#undef LUA_MAXUNSIGNED
#define LUA_MAXUNSIGNED (UINT_MAX)

#undef EPSILON
#define EPSILON (4.94065645841247E-324)

#undef EPSILONF
#define EPSILONF (1.401298E-45)

#undef MOD
#define MOD (1000003)

#undef FUZZY_EPSILON
#define FUZZY_EPSILON (1e-30)

#undef FUZZY_EPSILONF
#define FUZZY_EPSILONF (1e-6)

#undef EULER   
#define EULER (2.7182818284590452353602874713527)

#undef LOG10E
#define LOG10E (0.4342945f)

#undef LOG2E
#define LOG2E (1.442695f)

#undef SQRT_FIVE
#define SQRT_FIVE sqrt(5)

#undef GOLDEN_RATIO
#define GOLDEN_RATIO ((1 + SQRT_FIVE) / 2)

#undef PI
#define PI (3.141592653589793238462643383279502884)
#define RADIANS_PER_DEGREE (PI / 180.0)

#undef TAU
#define TAU (PI * 2)

#undef ETA
#define ETA (PI / 2)

#undef PIOVER4
#define PIOVER4 (PI / 4)


static int math_abs (lua_State *L) {
  lua_pushnumber(L, fabs(luaL_checknumber(L, 1)));
  return 1;
}

static int math_sin (lua_State *L) {
  lua_pushnumber(L, sin(luaL_checknumber(L, 1)));
  return 1;
}

static int math_sinh (lua_State *L) {
  lua_pushnumber(L, sinh(luaL_checknumber(L, 1)));
  return 1;
}

static int math_cos (lua_State *L) {
  lua_pushnumber(L, cos(luaL_checknumber(L, 1)));
  return 1;
}

static int math_cosh (lua_State *L) {
  lua_pushnumber(L, cosh(luaL_checknumber(L, 1)));
  return 1;
}

static int math_tan (lua_State *L) {
  lua_pushnumber(L, tan(luaL_checknumber(L, 1)));
  return 1;
}

static int math_tanh (lua_State *L) {
  lua_pushnumber(L, tanh(luaL_checknumber(L, 1)));
  return 1;
}

static int math_asin (lua_State *L) {
  lua_pushnumber(L, asin(luaL_checknumber(L, 1)));
  return 1;
}

static int math_asinh (lua_State *L) {
  lua_pushnumber(L, asinh(luaL_checknumber(L, 1)));
  return 1;
}

static int math_acos (lua_State *L) {
  lua_pushnumber(L, acos(luaL_checknumber(L, 1)));
  return 1;
}

static int math_atan (lua_State *L) {
  lua_pushnumber(L, atan(luaL_checknumber(L, 1)));
  return 1;
}

static int math_atan2 (lua_State *L) {
  lua_pushnumber(L, atan2(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_ceil (lua_State *L) {
  lua_pushnumber(L, ceil(luaL_checknumber(L, 1)));
  return 1;
}

static int math_floor (lua_State *L) {
  lua_pushnumber(L, floor(luaL_checknumber(L, 1)));
  return 1;
}

static int math_fmod (lua_State *L) {
  lua_pushnumber(L, fmod(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_modf (lua_State *L) {
  double ip;
  double fp = modf(luaL_checknumber(L, 1), &ip);
  lua_pushnumber(L, ip);
  lua_pushnumber(L, fp);
  return 2;
}

static int math_sqr (lua_State *L) {
  double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, x * x);
  return 1;
}

static int math_sqrt (lua_State *L) {
  lua_pushnumber(L, sqrt(luaL_checknumber(L, 1)));
  return 1;
}

static int math_invsqrt(lua_State* L)
{
    lua_pushnumber(L, 1.0 / sqrt(luaL_checknumber(L, 1)));
    return 1;
}

static int math_cbrt(lua_State* L)
{
  lua_pushnumber(L, cbrt(luaL_checknumber(L, 1)));
  return 1;
}

static int math_root(lua_State* L)
{
  double n = luaL_checknumber(L, 1);
  double base = luaL_checknumber(L, 2);
  if (base < 1 || ((n - floor(n / 2) * 2) == 0 && n < 0))
      lua_pushnumber(L, NAN);
  else
      lua_pushnumber(L, pow(n, (1 / base)));
  return 1;
}

static int math_approximately(lua_State* L)
{
	lua_pushboolean(L, (fabs(luaL_checknumber(L, 1) - luaL_checknumber(L, 2)) < FUZZY_EPSILONF));
  return 1;
}

static int math_ult (lua_State *L) {
  int a = luaL_checkinteger(L, 1);
  int b = luaL_checkinteger(L, 2);
  lua_pushboolean(L, (unsigned int)a < (unsigned int)b);
  return 1;
}

static int math_pow (lua_State *L) {
  lua_pushnumber(L, pow(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_log (lua_State *L) {
  double x = luaL_checknumber(L, 1);
  double res;
  if (lua_isnoneornil(L, 2))
    res = log(x);
  else
  {
    double base = luaL_checknumber(L, 2);
    if (base == 2.0)
      res = log2(x);
    else if (base == 10.0)
      res = log10(x);
    else
      res = log(x) / log(base);
  }
  lua_pushnumber(L, res);
  return 1;
}

static int math_ln(lua_State* L)
{
  lua_pushnumber(L, log(luaL_checknumber(L, 1)) / log(EULER));
  return 1;
}

static int math_log10 (lua_State *L) {
  lua_pushnumber(L, log10(luaL_checknumber(L, 1)));
  return 1;
}

static int math_log1p(lua_State* L)
{
  lua_pushnumber(L, log1p(luaL_checknumber(L, 1)));
  return 1;
}

static int math_log2(lua_State* L)
{
  lua_pushnumber(L, log2(luaL_checknumber(L, 1)));
  return 1;
}

static int math_exp (lua_State *L) {
  lua_pushnumber(L, exp(luaL_checknumber(L, 1)));
  return 1;
}

static int math_exp2(lua_State* L)
{
  lua_pushnumber(L, exp2(luaL_checknumber(L, 1)));
  return 1;
}

static int math_expm1(lua_State* L)
{
  lua_pushnumber(L, expm1(luaL_checknumber(L, 1)));
  return 1;
}

static int math_erf(lua_State* L)
{
  lua_pushnumber(L, erf(luaL_checknumber(L, 1)));
  return 1;
}

static int math_erfc(lua_State* L)
{
  lua_pushnumber(L, erfc(luaL_checknumber(L, 1)));
  return 1;
}

static int math_deg (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1)/RADIANS_PER_DEGREE);
  return 1;
}

static int math_rad (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1)*RADIANS_PER_DEGREE);
  return 1;
}

static int math_frexp (lua_State *L) {
  int e;
  lua_pushnumber(L, frexp(luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}

static int math_hypot(lua_State* L)
{
  lua_pushnumber(L, hypot(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_ilogb(lua_State* L)
{
  lua_pushinteger(L, ilogb(luaL_checknumber(L, 1)));
  return 1;
}

static int math_logb(lua_State* L)
{
  lua_pushnumber(L, logb(luaL_checknumber(L, 1)));
  return 1;
}

static int math_ldexp (lua_State *L) {
  lua_pushnumber(L, ldexp(luaL_checknumber(L, 1), luaL_checkint(L, 2)));
  return 1;
}

static int math_remainder(lua_State* L)
{
  lua_pushnumber(L, remainder(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_remquo(lua_State* L)
{
  int quot;
  lua_pushnumber(L, remquo(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &quot));
  lua_pushinteger(L, quot);
  return 2;
}

static int math_min (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  lua_Number dmin = luaL_checknumber(L, 1);
  int i;
  for (i=2; i<=n; i++) {
    lua_Number d = luaL_checknumber(L, i);
    if (d < dmin)
      dmin = d;
  }
  lua_pushnumber(L, dmin);
  return 1;
}


static int math_max (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  lua_Number dmax = luaL_checknumber(L, 1);
  int i;
  for (i=2; i<=n; i++) {
    lua_Number d = luaL_checknumber(L, i);
    if (d > dmax)
      dmax = d;
  }
  lua_pushnumber(L, dmax);
  return 1;
}

static int math_nearbyint(lua_State* L)
{
    lua_pushnumber(L, nearbyint(luaL_checknumber(L, 1)));
    return 1;
}

static int math_random (lua_State *L) {
  /* the `%' avoids the (rare) case of r==1, and is needed also because on
     some systems (SunOS!) `rand()' may return a value larger than RAND_MAX */
  lua_Number r = (lua_Number)(rand()%RAND_MAX) / (lua_Number)RAND_MAX;
  switch (lua_gettop(L)) {  /* check number of arguments */
    case 0: {  /* no arguments */
      lua_pushnumber(L, r);  /* Number between 0 and 1 */
      break;
    }
    case 1: {  /* only upper limit */
      int u = luaL_checkint(L, 1);
      luaL_argcheck(L, 1<=u, 1, "interval is empty");
      lua_pushnumber(L, floor(r*u)+1);  /* int between 1 and `u' */
      break;
    }
    case 2: {  /* lower and upper limits */
      int l = luaL_checkint(L, 1);
      int u = luaL_checkint(L, 2);
      luaL_argcheck(L, l<=u, 2, "interval is empty");
      lua_pushnumber(L, floor(r*(u-l+1))+l);  /* int between `l' and `u' */
      break;
    }
    default: return luaL_error(L, "wrong number of arguments");
  }
  return 1;
}


static int math_randomseed (lua_State *L) {
  srand(luaL_checkint(L, 1));
  return 0;
}


static int math_type (lua_State *L) {
  if (lua_type(L, 1) == LUA_TNUMBER) {
    double n = lua_tonumber(L, 1);
    if (n == (int)n)
      lua_pushliteral(L, "integer");
    else
      lua_pushliteral(L, "float");
  }
  else {
    luaL_checkany(L, 1);
    lua_pushnil(L);
  }
  return 1;
}

static const lu_byte kPerlin[512] = {151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99,
  37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
  20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
  55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86,
  164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17,
  182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110,
  79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14,
  239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24,
  72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,

  151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247,
  120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
  165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65,
  25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52,
  217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213,
  119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112,
  104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199,
  106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61,
  156, 180};

static float fade(float t)
{
  return t * t * t * (t * (t * 6 - 15) + 10);
}

static float lerp(float t, float a, float b)
{
  return a + t * (b - a);
}

static float grad(lu_byte hash, float x, float y, float z)
{
  lu_byte h = hash & 15;
  float u = (h < 8) ? x : y;
  float v = (h < 4) ? y : (h == 12 || h == 14) ? x : z;

  return (h & 1 ? -u : u) + (h & 2 ? -v : v);
}

static float perlin(float x, float y, float z)
{
  float xflr = floorf(x);
  float yflr = floorf(y);
  float zflr = floorf(z);

  int xi = (int)xflr & 255;
  int yi = (int)yflr & 255;
  int zi = (int)zflr & 255;

  float xf = x - xflr;
  float yf = y - yflr;
  float zf = z - zflr;

  float u = fade(xf);
  float v = fade(yf);
  float w = fade(zf);

  const lu_byte* p = kPerlin;

  int a = p[xi] + yi;
  int aa = p[a] + zi;
  int ab = p[a + 1] + zi;

  int b = p[xi + 1] + yi;
  int ba = p[b] + zi;
  int bb = p[b + 1] + zi;

  return lerp(w,
      lerp(v, lerp(u, grad(p[aa], xf, yf, zf), grad(p[ba], xf - 1, yf, zf)), lerp(u, grad(p[ab], xf, yf - 1, zf), grad(p[bb], xf - 1, yf - 1, zf))),
      lerp(v, lerp(u, grad(p[aa + 1], xf, yf, zf - 1), grad(p[ba + 1], xf - 1, yf, zf - 1)),
      lerp(u, grad(p[ab + 1], xf, yf - 1, zf - 1), grad(p[bb + 1], xf - 1, yf - 1, zf - 1))));
}

static int math_grad(lua_State* L)
{
  lu_byte hash = cast_byte(luaL_checkinteger(L, 1));
  double x = luaL_checknumber(L, 2);
  double y = luaL_checknumber(L, 3);
  double z = luaL_checknumber(L, 4);

  lu_byte h = hash & 15;
  double u = (h < 8) ? x : y;
  double v = (h < 4) ? y : (h == 12 || h == 14) ? x : z;
  double r = (h & 1 ? -u : u) + (h & 2 ? -v : v);

  lua_pushnumber(L, r);

  return 1;
}

static int math_fade(lua_State* L)
{
  double t = luaL_checknumber(L, 1);

  double r = t * t * t * (t * (t * 6 - 15) + 10);

  lua_pushnumber(L, r);

  return 1;
}

static int math_lerp(lua_State* L)
{
  double t = luaL_checknumber(L, 1);
  double a = luaL_checknumber(L, 2);
  double b = luaL_checknumber(L, 3);
  int precise = luaL_optboolean(L, 4, 0);

  double r = precise ? ((1 - t) * a + t * b) : (a + t * (b - a));

  lua_pushnumber(L, r);

  return 1;
}

static int math_noise(lua_State* L)
{
  double x = luaL_checknumber(L, 1);
  double y = luaL_optnumber(L, 2, 0.0);
  double z = luaL_optnumber(L, 3, 0.0);

  double r = perlin((float)x, (float)y, (float)z);

  lua_pushnumber(L, r);

  return 1;
}

static int math_clamp(lua_State* L)
{
  double v = luaL_checknumber(L, 1);
  double min = luaL_checknumber(L, 2);
  double max = luaL_checknumber(L, 3);

  luaL_argcheck(L, min <= max, 3, "max must be greater than or equal to min");

  double r = v < min ? min : v;
  r = r > max ? max : r;

  lua_pushnumber(L, r);
  return 1;
}

static int math_sign(lua_State* L)
{
  double v = luaL_checknumber(L, 1);
  lua_pushinteger(L, v > 0.0 ? 1 : v < 0.0 ? -1 : 0);
  return 1;
}

static int math_signbit(lua_State* L)
{
  double num = luaL_checknumber(L, 1);
  if (num < 0.0)
    lua_pushinteger(L, 1);
  else
    lua_pushinteger(L, 0);
  return 1;
}

static int math_copysign(lua_State* L)
{
  lua_pushnumber(L, copysign(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_round(lua_State* L)
{
  lua_pushnumber(L, round(luaL_checknumber(L, 1)));
  return 1;
}

static int math_scalbn(lua_State* L)
{
  lua_pushnumber(L, scalbn(luaL_checknumber(L, 1), luaL_checkinteger(L, 2)));
  return 1;
}

static int math_tgamma(lua_State* L)
{
  lua_pushnumber(L, tgamma(luaL_checknumber(L, 1)));
  return 1;
}

static int math_lgamma(lua_State* L)
{
  lua_pushnumber(L, lgamma(luaL_checknumber(L, 1)));
  return 1;
}

static int math_trunc(lua_State* L)
{
  lua_pushnumber(L, trunc(luaL_checknumber(L, 1)));
  return 1;
}

static int math_fdim(lua_State* L)
{
  lua_pushnumber(L, fdim(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_fma(lua_State* L)
{
  lua_pushnumber(L, fma(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
  return 1;
}

static int math_nextafter(lua_State* L)
{
  lua_pushnumber(L, nextafter(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_nextforward(lua_State* L)
{
  #ifdef nextforward
  lua_pushnumber(L, nextforward(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  #else
  lua_pushnumber(L, nextafter(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  #endif
  return 1;
}

static int math_classify(lua_State* L)
{
  switch (fpclassify(luaL_checknumber(L, 1))) {
      case FP_INFINITE:
        lua_pushliteral(L, "infinite");
        break;
      case FP_NAN:
        lua_pushliteral(L, "NaN");
        break;
      case FP_ZERO:
        lua_pushliteral(L, "zero");
        break;
      case FP_SUBNORMAL:
        lua_pushliteral(L, "subnormal");
        break;
      case FP_NORMAL:
        lua_pushliteral(L, "normal");
        break;
      default:
        lua_pushnil(L);
        break;
  }
  return 1;
}

#ifdef MISSING_ISINF
#define isinf(x) (!isnan(x) && isnan((x) - (x)))
#endif

static int math_isinf(lua_State* L)
{
  lua_pushboolean(L, isinf(luaL_checknumber(L, 1)));
  return 1;
}

static int math_isfinite(lua_State* L)
{
  lua_pushboolean(L, isfinite(luaL_checknumber(L, 1)));
  return 1;
}

static int math_isnormal(lua_State* L)
{
  lua_pushboolean(L, isnormal(luaL_checknumber(L, 1)));
  return 1;
}

static int math_isnan(lua_State* L)
{
  lua_pushboolean(L, isnan(luaL_checknumber(L, 1)));
  return 1;
}

static int math_isunordered(lua_State* L)
{
  lua_pushboolean(L, isunordered(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int math_toint(lua_State* L)
{
  int valid;
  int n = lua_tointegerx(L, 1, &valid);
  if (valid)
      lua_pushinteger(L, n);
  else {
      luaL_checkany(L, 1);
      lua_pushnil(L);  /* value is not convertible to integer */
  }
  return 1;
}

double exponentMod(double a, double b, double m)
{
    // Base cases
    if (a == 0)
        return 0;
    if (b == 0)
        return 1;
 
    // If B is even
    double y;
    if (fmod(b, 2) == 0)
    {
        y = exponentMod(a, b / 2, m);
        y = fmod((y * y), m);
    }
 
    // If B is odd
    else
    {
        y = fmod(a, m);
        y = fmod((y * fmod(exponentMod(a, b - 1, m), m)), m);
    }
 
    return fmod((y + m), m);
}

static int math_powmod(lua_State* L)
{
    lua_pushnumber(L, exponentMod(luaL_checknumber(L, 1),luaL_checknumber(L, 2),luaL_checknumber(L, 3)));
    return 1;
}

static int math_invmod(lua_State* L)
{
    double a = luaL_checknumber(L, 1);
    double m = luaL_checknumber(L, 2);
    for (int x = 1; x < m; x++)
    {
        if (fmod((fmod(a, m) * fmod(x, m)), m) == 1)
        {
            lua_pushinteger(L, x);
            return 1;
        }
    }
    lua_pushnil(L);
    return 1;
}

static int math_rep(lua_State* L)
{
  double t = luaL_checknumber(L, 1);
  double length = luaL_checknumber(L, 2);
  lua_pushnumber(L, (t - (floor(t / length) * length)));
  return 1;
}

static double eps(double a, double b, double e)
{
	double aa = fabs(a) + 1;
  if (isinf(aa))
      return e;
  else
      return e * aa;
}

static int math_eps(lua_State* L)
{
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  double e = luaL_optnumber(L, 3, FUZZY_EPSILONF);
  lua_pushnumber(L, eps(a, b, e));
  return 1;
}

static int math_fuzzyEq(lua_State* L)
{
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  double e = luaL_optnumber(L, 3, FUZZY_EPSILONF);
  lua_pushboolean(L, (a == b || fabs(a - b) <= eps(a, b, e)));
  return 1;
}

static int math_fuzzyNe(lua_State* L)
{
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  double e = luaL_optnumber(L, 3, FUZZY_EPSILONF);
  lua_pushboolean(L, (a == b || fabs(a - b) <= eps(a, b, e)) == 0);
  return 1;
}
 
static int math_fuzzyGt(lua_State* L)
{
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  double e = luaL_optnumber(L, 3, FUZZY_EPSILONF);
  lua_pushboolean(L, (a > b + eps(a, b, e)));
  return 1;
}
 
static int math_fuzzyGe(lua_State* L)
{
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  double e = luaL_optnumber(L, 3, FUZZY_EPSILONF);
  lua_pushboolean(L, (a > b - eps(a, b, e)));
  return 1;
}
 
static int math_fuzzyLt(lua_State* L)
{
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  double e = luaL_optnumber(L, 3, FUZZY_EPSILONF);
  lua_pushboolean(L, (a < b - eps(a, b, e)));
  return 1;
}
 
static int math_fuzzyLe(lua_State* L)
{
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  double e = luaL_optnumber(L, 3, FUZZY_EPSILONF);
  lua_pushboolean(L, (a < b + eps(a, b, e)));
  return 1;
}

static int math_quadratic(lua_State* L)
{
  // Coefficients of the quadratic equation
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  double c = luaL_checknumber(L, 3);

  // Compute the roots of the quadratic equation
  double discriminant = (b * b) - (4 * a * c);

  // Check if the equation has real roots
  if (discriminant > 0) {
    // If the discriminant is positive, the equation has two real roots
    lua_pushnumber(L, (-b + sqrt(discriminant)) / (2 * a));
    lua_pushnumber(L, (-b - sqrt(discriminant)) / (2 * a));
    return 2;
  } else if (discriminant == 0) {
    // If the discriminant is zero, the equation has one real root
    lua_pushnumber(L, -b / (2 * a));
  } else {
    lua_pushnil(L);
  }
  return 1;
}

static int math_iseven(lua_State* L)
{
  lua_pushboolean(L, luaL_checkinteger(L, 1) % 2 == 0);
  return 1;
}

static int math_isodd(lua_State* L)
{
  lua_pushboolean(L, luaL_checkinteger(L, 1) % 2 != 0);
  return 1;
}

static int math_bin(lua_State* L)
{
  int n = luaL_checkinteger(L, 1), i;
  for(i = 0; n > 0; i++)
  {
    lua_pushinteger(L, n % 2);
    n = n / 2;
  }
  return i;
}

// Function to check whether the given number is a factorial of some number or not
static int math_isfact(lua_State* L)
{
    unsigned n = luaL_checkinteger(L, 1);
    if (n <= 0) {
        lua_pushboolean(L, false);
        return 1;
    }
    for (unsigned i = 1;; i++) {
        if (n % i != 0)
            break;
        n = n / i;
    }
    
    lua_pushboolean(L, n == 1);
    return 1;
}

/** function to find factorial of given number */
unsigned factorial(unsigned n, unsigned step) {
    unsigned res = 1;
    for (unsigned i = n;; i -= step) {
        if (i == 0 || i == 1)
            return res;
        res *= i;
    }
    return res;
}

static int math_fact(lua_State* L)
{
    int n = luaL_checkinteger(L, 1);

    // no negative integers
    if (n < 0){
      luaL_argerror(L, 1, "factorial of a negative integer doesn't exist");
    }

    if (n < 17)
    {
        lua_pushinteger(L, factorial(n, luaL_optinteger(L, 2, 1))); 
        return 1;
    }
    else
    {
        lua_pushnumber(L, HUGE_VAL); 
        return 1;
    }
}

// Recursive function to return gcd of a and b
static double gcd(double a, double b)
{
    if (a < b)
        return gcd(b, a);
 
    // base case
    if (b < 0.001)
        return a;
    else
        return (gcd(b, a - floor(a / b) * b));
}

static int math_gcd(lua_State* L)
{
    int n = lua_gettop(L); /* number of arguments */
    if (n == 1)
    {
        double a = fabs(luaL_checknumber(L, 1));
        lua_pushnumber(L, gcd(a,a));
    }
    else if (n == 2)
    {
        double a = fabs(luaL_checknumber(L, 1));
        double b = fabs(luaL_checknumber(L, 2));
        lua_pushnumber(L, gcd(a,b));
    }
    else
    {
        double result = fabs(luaL_checknumber(L, 1));
        for (int i = 2; i <= n; i++)
        {
            double a = fabs(luaL_checknumber(L, i));
            result = gcd(a, result);
    
            if(result == 1)
            {
                lua_pushinteger(L, 1);
                return 1;
            }
        }
        lua_pushnumber(L, result);
    }
    return 1;
}

// Function to return LCM of two numbers
static double lcm(double a, double b)
{
    return (a / gcd(a, b)) * b;
}

static int math_lcm(lua_State* L)
{
    int n = lua_gettop(L); /* number of arguments */
    if (n == 1)
    {
        double a = fabs(luaL_checknumber(L, 1));
        lua_pushnumber(L, lcm(a,a));
    }
    else if (n == 2)
    {
        double a = fabs(luaL_checknumber(L, 1));
        double b = fabs(luaL_checknumber(L, 2));
        lua_pushnumber(L, lcm(a,b));
    }
    else
    {
        double ans = fabs(luaL_checknumber(L, 1));
        for (int i = 2; i <= n; i++){
            double a = fabs(luaL_checknumber(L, i));
            ans = (((a * ans)) / (gcd(a, ans)));
        }
        lua_pushnumber(L, ans);
    }
    return 1;
}

static int math_digit(lua_State* L)
{
  int n = luaL_checkinteger(L, 1);
  int count = 0;
 
  // iterate at least once, then until n becomes 0
  // remove last digit from n in each iteration
  // increase count by 1 in each iteration
  do {
    n /= 10;
    ++count;
  } while (n != 0);
  
  lua_pushinteger(L, count);
  return 1;
}

// Function to calculate order of the number
static int math_ispalindrome(lua_State* L)
{
  int n = luaL_checkinteger(L, 1);
  int reversed = 0;
  int original = n;
  // reversed integer is stored in reversed variable
  while (n != 0)
  {
    int remainder = n % 10;
    reversed = reversed * 10 + remainder;
    n /= 10;
  }
  // palindrome if original and reversed are equal
  lua_pushboolean(L, original == reversed);
  return 1;
}

static int math_isprime(lua_State* L){
  int n = luaL_checkinteger(L, 1);
  int flag = true;

  // 0 and 1 are not prime numbers
  if (n == 0 || n == 1)
    flag = false;
  else {
    for (int i = 2; i <= n / 2; ++i) {
      // if n is divisible by i, then n is not prime
      if (n % i == 0) {
        flag = false;
        break;
      }
    }
  }

  lua_pushboolean(L, flag);
  return 1;
}

static int math_isarmstrong(lua_State* L){
  int num = luaL_checkinteger(L, 1);
  int originalNum = num;
  int remainder;
  int n = 0;
  float result = 0.0;

  // store the number of digits of num in n
  for (originalNum = num; originalNum != 0; ++n) {
       originalNum /= 10;
  }

  for (originalNum = num; originalNum != 0; originalNum /= 10) {
    remainder = originalNum % 10;

    // store the sum of the power of individual digits in result
    result += pow(remainder, n);
  }

  // if num is equal to result, the number is an Armstrong number
  lua_pushboolean(L, (int)result == num);
  return 1;
}


static int math_tostring(lua_State* L)
{
    double val = luaL_checknumber(L, 1);
    
    // Check for NaN, -inf and inf
    if (isnan(val)){
        lua_pushliteral(L, "NaN");
        return 1;
    }
    else if (isinf(val)){
        if (val > 0.0)
            lua_pushliteral(L, "inf");
        else
            lua_pushliteral(L, "-inf");
        return 1;
    }

    luaL_Buffer b;
    luaL_buffinit(L, &b);
    char buff[512];   /* to store the formatted item */
    sprintf(buff, "%.f", val);
    luaL_addlstring(&b, buff, strlen(buff));
    luaL_pushresult(&b);
    
    return 1;
}

int sum_of_divisor(int num) {
    // Variable to store the sum of all proper divisors.
    int sum = 0;
    // Below loop condition helps to reduce Time complexity by a factor of
    // square root of the number.
    for (int div = 2; div * div <= num; ++div) {
        // Check 'div' is divisor of 'num'.
        if (num % div == 0) {
            // If both divisor are same, add once to 'sum'
            if (div == (num / div)) {
                sum += div;
            } else {
                // If both divisor are not the same, add both to 'sum'.
                sum += (div + (num / div));
            }
        }
    }
    return sum + 1;
}

// Function to calculate the sum of all the proper divisor
static int math_sumdivisor(lua_State* L)
{
    lua_pushinteger(L, sum_of_divisor(luaL_checkinteger(L, 1)));
    return 1;
}

// Function to check whether the pair is amicable or not.
static int math_amicable(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    lua_pushboolean(L, (sum_of_divisor(x) == y) && (sum_of_divisor(y) == x));
    return 1;
}

// Function to find reverse of any number
static int math_reverse(lua_State* L)
{
    int number = luaL_checkinteger(L, 1);
    int reversedNumber = 0;
    while (number != 0)
    {
        int remainder = number % 10;
        reversedNumber = reversedNumber * 10 + remainder;
        number /= 10;
    }
    lua_pushinteger(L, reversedNumber);
    return 1;
}

// Function to find n'th carol number
static int math_carol(lua_State* L)
{
    int n = luaL_checkinteger(L, 1);
    int an = abs(n);
    int result = pow(2, an) - 1;
    result = result * result - 2;
    if (an != n)
        lua_pushinteger(L, -result);
    else
        lua_pushinteger(L, result);
    return 1;
}

static unsigned permute(unsigned n, unsigned k)
{
    unsigned result = 1;

    for (; n > k; --n)
        result *= n;

    return result;
}

static int math_permute(lua_State* L)
{
    lua_pushinteger(L, permute(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static inline unsigned binomial(unsigned n, unsigned k)
{
    if ( n - k > k )
        return permute(n, n - k) / permute(k, 1);
    else
        return permute(n, k) / permute(n - k, 1);
}

static int math_binomial(lua_State* L)
{
    lua_pushinteger(L, binomial(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int math_fib(lua_State* L)
{
    int n = luaL_checkinteger(L, 1);
    int an = abs(n);
    if (an > 46)
      lua_pushnumber(L, HUGE_VAL);
    else {
      double numerator = pow(GOLDEN_RATIO, (double)n) - pow(1-GOLDEN_RATIO, (double)n);
      double denominator = SQRT_FIVE;
      // This cast should in general work, as the result is always an integer. 
      // Floating point errors may occur!
      lua_pushinteger(L, (int)(numerator/denominator)); 
    }
    return 1;
}

// Returns value of Binomial Coefficient C(n, k)
static int binomialCoeff(int n, int k)
{
    // Base Cases
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
  
    // Recur
    return binomialCoeff(n - 1, k - 1)
           + binomialCoeff(n - 1, k);
}

static int math_binomcoef(lua_State* L)
{
  lua_pushinteger(L, binomialCoeff(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
  return 1;
}

static int math_mean(lua_State* L)
{
    int n = lua_gettop(L); /* number of arguments */
    double sum = 0;

    for (int i = 1; i <= n; i++)
    {
        double a = luaL_checknumber(L, i);
        sum = sum + a;
    }

    lua_pushnumber(L, sum / (double)n);
    return 1;
}

static int math_compare(lua_State* L)
{
    double a = luaL_checknumber(L, 1);
    double b = luaL_checknumber(L, 2);
    if (a == b)
        lua_pushinteger(L, 0);
    else if (a > b)
        lua_pushinteger(L, 1);
    else
        lua_pushinteger(L, -1);
    return 1;
}

static int math_mantissa(lua_State* L)
{
    int e;
    lua_pushnumber(L, fabs(frexp(luaL_checknumber(L, 1), &e)));
    return 1;
}

static int math_exponent(lua_State* L)
{
    int e;
    frexp(luaL_checknumber(L, 1), &e);
    lua_pushinteger(L, e);
    return 1;
}

static int math_dist(lua_State* L)
{
    lua_Number x1 = luaL_checknumber(L, 1);
    lua_Number y1 = luaL_checknumber(L, 2);
    lua_Number x2 = luaL_checknumber(L, 3);
    lua_Number y2 = luaL_checknumber(L, 4);
	  lua_Number xd = x2 - x1;
	  lua_Number yd = y2 - y1;
	
    lua_pushnumber(L, sqrt(xd * xd + yd * yd));
    return 1;
}

static int math_normalizeangle(lua_State* L)
{
    lua_pushinteger(L, (luaL_checkinteger(L, 1) + 180) % 360 - 180);
    return 1;
}

static int math_inrange(lua_State* L)
{
    lua_Number a = luaL_checknumber(L, 1);
    lua_Number min = luaL_checknumber(L, 2);
    lua_Number max = luaL_checknumber(L, 3);
    lua_pushboolean(L, a >= min && a <= max);
    return 1;
}

static lua_Number clamp (lua_Number var, lua_Number min, lua_Number max){
	if (var < min) return min;
	if (var > max) return max;
  return var;
}

static int math_approach(lua_State* L)
{
    lua_Number cur = luaL_checknumber(L, 1);
    lua_Number target = luaL_checknumber(L, 2);
    lua_Number inc = fabs(luaL_checknumber(L, 3));

    if (cur < target)
      lua_pushnumber(L, clamp(cur + inc, cur, target));
    else if (cur > target)
      lua_pushnumber(L, clamp(cur - inc, target, cur));
    else
      lua_pushnumber(L, target);

    return 1;
}

static const luaL_Reg mathlib[] = {
  {"abs",   math_abs},
  {"acos",  math_acos},
  {"approach",  math_approach},
  {"asin",  math_asin},
  {"asinh",  math_asinh},
  {"atan2", math_atan2},
  {"atan",  math_atan},
  {"amicable", math_amicable},
  {"approximately", math_approximately},
  {"binary",  math_bin},
  {"binom",  math_binomial},
  {"binomcoef",  math_binomcoef},
  {"carol", math_carol},
  {"cbrt",  math_cbrt},
  {"ceil",  math_ceil},
  {"classify", math_classify},
  {"compare", math_compare},
  {"copysign", math_copysign},
  {"cosh",   math_cosh},
  {"cos",   math_cos},
  {"deg",   math_deg},
  {"digit", math_digit},
  {"dist",   math_dist},
  {"eps",  math_eps},
  {"erf",  math_erf},
  {"erfc", math_erfc},
  {"exponent", math_exponent},
  {"exp",   math_exp},
  {"exp2", math_exp2},
  {"expm1", math_expm1},
  {"fade", math_fade},
  {"fdim", math_fdim},
  {"fib", math_fib},
  {"fact", math_fact},
  {"floor", math_floor},
  {"fma",  math_fma},
  {"fmod",   math_fmod},
  {"fuzzyeq", math_fuzzyEq},
  {"fuzzyge", math_fuzzyGe},
  {"fuzzygt", math_fuzzyGt},
  {"fuzzyle", math_fuzzyLe},
  {"fuzzylt", math_fuzzyLt},
  {"fuzzyne", math_fuzzyNe},
  {"frexp", math_frexp},
  {"gcd", math_gcd},
  {"grad", math_grad},
  {"hcf", math_gcd},
  {"hypot", math_hypot},
  {"ilogb", math_ilogb},
  {"inrange",  math_inrange},
  {"invmod", math_invmod},
  {"invsqrt", math_invsqrt},
  {"isarmstrong", math_isarmstrong},
  {"iseven", math_iseven},
  {"isinf", math_isinf},
  {"isfact", math_isfact},
  {"isfinite", math_isfinite},
  {"isnormal", math_isnormal},
  {"isnan", math_isnan},
  {"isodd", math_isodd},
  {"ispalindrome", math_ispalindrome},
  {"isprime", math_isprime},
  {"isunordered", math_isunordered},
  {"lcm", math_lcm},
  {"ldexp", math_ldexp},
  {"lerp", math_lerp},
  {"lgamma", math_lgamma},
  {"ln", math_ln},
  {"log10", math_log10},
  {"log1p", math_log1p},
  {"log",   math_log},
  {"logb",  math_logb},
  {"log2",  math_log2},
  {"mantissa", math_mantissa},
  {"max",   math_max},
  {"mean", math_mean},
  {"min",   math_min},
  {"modf",   math_modf},
  {"nearbyint", math_nearbyint},
  {"nextafter", math_nextafter},
  {"nexttoward", math_nextforward},
  {"normdeg",   math_normalizeangle},
  {"permute", math_permute},
  {"pow",   math_pow},
  {"powmod", math_powmod},
  {"quadratic", math_quadratic},
  {"rad",   math_rad},
  {"random",     math_random},
  {"randomseed", math_randomseed},
  {"remainder", math_remainder},
  {"remquo", math_remquo},
  {"rep", math_rep},
  {"reverse", math_reverse},
  {"root", math_root},
  {"scalbn", math_scalbn},
  {"significand", math_mantissa},
  {"sinh",   math_sinh},
  {"sin",   math_sin},
  {"sqr",  math_sqr},
  {"sqrt",  math_sqrt},
  {"sumdivisor", math_sumdivisor},
  {"tanh",   math_tanh},
  {"tan",   math_tan},
  {"tgamma", math_tgamma},
  {"tointeger", math_toint},
  {"tostring", math_tostring},
  {"trunc", math_trunc},
  {"type", math_type},
  {"ult",   math_ult},
  {"noise", math_noise},
  {"clamp", math_clamp},
  {"sign", math_sign},
  {"signbit", math_signbit},
  {"round", math_round},
  {NULL, NULL}
};


/*
** Open math library
*/
LUALIB_API int luaopen_math (lua_State *L) {
  luaL_register(L, LUA_MATHLIBNAME, mathlib);
  lua_pushnumber(L, PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, TAU);
  lua_setfield(L, -2, "tau");
  lua_pushnumber(L, ETA);
  lua_setfield(L, -2, "eta");
  lua_pushnumber(L, HUGE_VAL);
  lua_setfield(L, -2, "huge");
  lua_pushnumber(L, EULER);
  lua_setfield(L, -2, "e");
  lua_pushnumber(L, LOG10E);
  lua_setfield(L, -2, "log10e");
  lua_pushnumber(L, LOG2E);
  lua_setfield(L, -2, "log2e");
  lua_pushnumber(L, EPSILON);
  lua_setfield(L, -2, "epsilon");
  lua_pushnumber(L, EPSILONF);
  lua_setfield(L, -2, "epsilonf");
  lua_pushnumber(L, FUZZY_EPSILON);
  lua_setfield(L, -2, "fuzzyepsilon");
  lua_pushnumber(L, FUZZY_EPSILONF);
  lua_setfield(L, -2, "fuzzyepsilonf");
  lua_pushnumber(L, NAN);
  lua_setfield(L, -2, "nan");
  lua_pushinteger(L, LUA_MAXINTEGER);
  lua_setfield(L, -2, "maxinteger");
  lua_pushinteger(L, LUA_MININTEGER);
  lua_setfield(L, -2, "mininteger");
  lua_pushinteger(L, LUA_MAXUNSIGNED);
  lua_setfield(L, -2, "maxunsigned");
  lua_pushnumber(L, GOLDEN_RATIO);
  lua_setfield(L, -2, "phi");
  lua_pushnumber(L, SQRT_FIVE);
  lua_setfield(L, -2, "sqrtfive");
#if defined(LUA_COMPAT_MOD)
  lua_getfield(L, -1, "fmod");
  lua_setfield(L, -2, "mod");
#else
  lua_pushnumber(L, MOD);
  lua_setfield(L, -2, "mod");
#endif
  return 1;
}

