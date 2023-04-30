#pragma once
#include "lua.hpp"

int nimo_luafn_HasEntityComponent(lua_State* L);
int nimo_luafn_GetEntityComponent(lua_State* L);
int nimo_luafn_SetEntityComponent(lua_State* L);