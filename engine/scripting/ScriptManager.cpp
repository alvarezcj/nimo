#include "ScriptManager.h"
#include "lua.hpp"
#include "ScriptUtils.h"
#include "core/Log.h"
#include "project/Project.h"
#include "scene/Scene.h"
#include "scene/Components.h"

lua_State* nimo::ScriptManager::L = nullptr;
const nimo::ScriptInstance* nimo::ScriptManager::processingInstance = nullptr;

int nimo_luafn_DebugLog(lua_State* L)
{
    std::string trace = luaL_checkstring(L, 1);
    NIMO_DEBUG("[LUA] {}", trace);
    return 0;
}
int nimo_luafn_WarningLog(lua_State* L)
{
    std::string trace = luaL_checkstring(L, 1);
    NIMO_WARN("[LUA] {}", trace);
    return 0;
}
int nimo_luafn_ErrorLog(lua_State* L)
{
    std::string trace = luaL_checkstring(L, 1);
    NIMO_ERROR("[LUA] {}", trace);
    return 0;
}
int nimo_luafn_GetEntityComponent(lua_State* L)
{
    // discard any extra arguments passed in
    nimo::ScriptUtils::PrintLuaStack(L);
    lua_settop(L, 1);
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "id");
    lua_getfield(L, 1, "scene");
    nimo::ScriptUtils::PrintLuaStack(L);
    if((lua_islightuserdata(L, -2) && lua_islightuserdata(L, -1)))
    {
        nimo::GUID* id = (nimo::GUID*)lua_touserdata(L, -2);
        nimo::Scene* scene = (nimo::Scene*)lua_touserdata(L, -1);
        lua_pushnumber(L, scene->GetEntity(*id).GetComponent<nimo::TransformComponent>().Rotation.x);
    }
    return 1;
}

void nimo::ScriptManager::Initialize()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    // Set asset folder as package path
    std::string code = "local script_path = [[" + Project::GetActiveProject()->GetAssetsFolderPath().string() + "\\?.lua]]\npackage.path = script_path";
    luaL_dostring(L, code.c_str());
    // Create nimo lib global table
    lua_newtable(L);
    // Debug
    {
        lua_newtable(L);
        lua_pushcfunction(L, nimo_luafn_DebugLog);
        lua_setfield(L, -2, "Log");
        lua_pushcfunction(L, nimo_luafn_WarningLog);
        lua_setfield(L, -2, "LogWarning");
        lua_pushcfunction(L, nimo_luafn_ErrorLog);
        lua_setfield(L, -2, "LogError");
        lua_setfield(L, -2, "Debug");
    }
    // Entity
    {
        lua_newtable(L);
        lua_pushcfunction(L, nimo_luafn_GetEntityComponent);
        lua_setfield(L, -2, "GetComponent");
        lua_setfield(L, -2, "Entity");
    }
    ScriptUtils::PrintLuaStack(L);
    lua_setglobal(L, "nimo");
}
void nimo::ScriptManager::Cleanup()
{
    lua_close(L);
}

nimo::ScriptInstance nimo::ScriptManager::CreateInstance(std::shared_ptr<Script> source, const GUID& owner, std::shared_ptr<Scene> scene)
{
    ScriptInstance res;
    if(luaL_dofile(L, source->filepath.c_str()) == LUA_OK)
    {
        ScriptUtils::PrintLuaStack(L);
        if(lua_istable(L, -1))
        {
            lua_newtable(L);
            lua_pushlightuserdata(L, (void*)&owner);
            lua_setfield(L, -2, "id");
            lua_pushlightuserdata(L, (void*)scene.get());
            lua_setfield(L, -2, "scene");
            lua_setfield(L, -2, "entity");
            ScriptUtils::PrintLuaStack(L);
            res.owner = owner;
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
    processingInstance = &instance;
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
    processingInstance = nullptr;
}
void nimo::ScriptManager::OnUpdate(const ScriptInstance& instance, float deltaTime)
{
    processingInstance = &instance;
    lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
    lua_pushstring(L, "OnUpdate");
    lua_gettable(L, -2);
    if(!lua_isnil(L, -1))
    {
        NIMO_DEBUG("Executing OnUpdate function");
        lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
        lua_pushnumber(L, deltaTime);
        lua_pcall(L, 2, 0, 0);
    }
    lua_pop(L, 1);
    processingInstance = nullptr;
}