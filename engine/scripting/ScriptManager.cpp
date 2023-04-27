#include "ScriptManager.h"
#include "lua.hpp"
#include "ScriptUtils.h"
#include "core/Log.h"

lua_State* nimo::ScriptManager::L = nullptr;

void nimo::ScriptManager::Initialize()
{
    L = luaL_newstate();
    luaL_openlibs(L);
}
void nimo::ScriptManager::Cleanup()
{
    lua_close(L);
}

nimo::ScriptInstance nimo::ScriptManager::CreateInstance(std::shared_ptr<Script> source)
{
    ScriptInstance res;
    if(luaL_dofile(L, source->filepath.c_str()) == LUA_OK)
    {
        ScriptUtils::PrintLuaStack(L);
        if(lua_istable(L, -1))
        {
            res.stackReference = luaL_ref(L, LUA_REGISTRYINDEX);
            res.script = source;
            NIMO_INFO("Registering script instance for {} with reference {}", source->filepath, res.stackReference);
            OnCreate(res);
        }
    }
    ScriptUtils::PrintLuaStack(L);
    return res;
}
void nimo::ScriptManager::OnCreate(const ScriptInstance& instance)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
    lua_pushstring(L, "OnCreate");
    lua_gettable(L, -2);
    if(!lua_isnil(L, -1))
    {
        NIMO_DEBUG("Executing OnCreate function");
        lua_pcall(L, 0, 0, 0);
    }
    else
    {
        NIMO_DEBUG("No OnCreate function");
    }
    lua_pop(L, 1);
}