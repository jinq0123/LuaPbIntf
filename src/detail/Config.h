#ifndef DETAIL_CONFIG_H
#define DETAIL_CONFIG_H

#include <lua.h>  // for LUA_VERSION_NUM

// Lua5.3 has 64-bit integers.
#if LUA_VERSION_NUM >= 503
#define LUA_HAS_64BIT_INTEGER 1
#endif

#endif  // DETAIL_CONFIG_H
