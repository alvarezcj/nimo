#include "LuaInput.h"
#include "input/Input.h"

int nimo_luafn_InputGetKey(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 1);
    int keycode = luaL_checkinteger(L, 1);
    lua_pushboolean(L, nimo::Input::GetKey((nimo::KeyCode)keycode));
    return 1;
}
int nimo_luafn_InputGetMouseButton(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 1);
    int button = luaL_checkinteger(L, 1);
    lua_pushboolean(L, nimo::Input::GetMouseButton((nimo::MouseButton)button));
    return 1;
}
int nimo_luafn_InputGetMousePosition(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 0);
    auto pos = nimo::Input::GetMousePosition();
    lua_pushnumber(L, pos.first);
    lua_pushnumber(L, pos.second);
    return 2;
}