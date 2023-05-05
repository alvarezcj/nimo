#include "LuaComponents.h"
#include "scene/Scene.h"
#include "scene/Components.h"
#include "assets/AssetType.h"
#include "assets/AssetManager.h"
#include "scene/SceneManager.h"
#include "scene/Prefab.h"

int nimo_luafn_HasEntityComponent(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 2);
    luaL_checktype(L, 1, LUA_TTABLE);
    std::string componentType = luaL_checkstring(L, 2);
    lua_getfield(L, 1, "id");
    lua_getfield(L, 1, "scene");
    if((lua_islightuserdata(L, -2) && lua_islightuserdata(L, -1)))
    {
        nimo::GUID* id = (nimo::GUID*)lua_touserdata(L, -2);
        nimo::Scene* scene = (nimo::Scene*)lua_touserdata(L, -1);
        if(componentType == "Transform")
        {
            lua_pushboolean(L, scene->GetEntity(*id).HasComponent<nimo::TransformComponent>());
        }
        else if(componentType == "Label")
        {
            lua_pushboolean(L, scene->GetEntity(*id).HasComponent<nimo::LabelComponent>());
        }
    }
    return 1;
}
int nimo_luafn_GetEntityComponent(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 2);
    luaL_checktype(L, 1, LUA_TTABLE);
    std::string componentType = luaL_checkstring(L, 2);
    lua_getfield(L, 1, "id");
    lua_getfield(L, 1, "scene");
    if((lua_islightuserdata(L, -2) && lua_islightuserdata(L, -1)))
    {
        nimo::GUID* id = (nimo::GUID*)lua_touserdata(L, -2);
        nimo::Scene* scene = (nimo::Scene*)lua_touserdata(L, -1);
        if(componentType == "Transform")
        {
            auto t = scene->GetEntity(*id).GetComponent<nimo::TransformComponent>();
            lua_newtable(L);
            // Translation
            {
                lua_newtable(L);
                lua_pushnumber(L, t.Translation.x);
                lua_setfield(L, -2, "x");
                lua_pushnumber(L, t.Translation.y);
                lua_setfield(L, -2, "y");
                lua_pushnumber(L, t.Translation.z);
                lua_setfield(L, -2, "z");
                lua_setfield(L, -2, "Translation");
            }
            // Rotation
            {
                lua_newtable(L);
                lua_pushnumber(L, t.Rotation.x);
                lua_setfield(L, -2, "x");
                lua_pushnumber(L, t.Rotation.y);
                lua_setfield(L, -2, "y");
                lua_pushnumber(L, t.Rotation.z);
                lua_setfield(L, -2, "z");
                lua_setfield(L, -2, "Rotation");
            }
            // Scale
            {
                lua_newtable(L);
                lua_pushnumber(L, t.Scale.x);
                lua_setfield(L, -2, "x");
                lua_pushnumber(L, t.Scale.y);
                lua_setfield(L, -2, "y");
                lua_pushnumber(L, t.Scale.z);
                lua_setfield(L, -2, "z");
                lua_setfield(L, -2, "Scale");
            }
        }
        else if(componentType == "Label")
        {
            lua_pushstring(L, scene->GetEntity(*id).GetComponent<nimo::LabelComponent>().Label.c_str());
        }
    }
    return 1;
}
int nimo_luafn_SetEntityComponent(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 3);
    luaL_checktype(L, 1, LUA_TTABLE);
    std::string componentType = luaL_checkstring(L, 2);
    luaL_checktype(L, 3, LUA_TTABLE);
    lua_getfield(L, 1, "id");
    lua_getfield(L, 1, "scene");
    if((lua_islightuserdata(L, -2) && lua_islightuserdata(L, -1)))
    {
        nimo::GUID* id = (nimo::GUID*)lua_touserdata(L, -2);
        nimo::Scene* scene = (nimo::Scene*)lua_touserdata(L, -1);
        if(componentType == "Transform")
        {
            auto& t = scene->GetEntity(*id).GetComponent<nimo::TransformComponent>();
            lua_getfield(L, 3, "Translation");
            lua_getfield(L, -1, "x");
            lua_getfield(L, -2, "y");
            lua_getfield(L, -3, "z");
            t.Translation.x = lua_tonumber(L, -3);
            t.Translation.y = lua_tonumber(L, -2);
            t.Translation.z = lua_tonumber(L, -1);
            lua_getfield(L, 3, "Rotation");
            lua_getfield(L, -1, "x");
            lua_getfield(L, -2, "y");
            lua_getfield(L, -3, "z");
            t.Rotation.x = lua_tonumber(L, -3);
            t.Rotation.y = lua_tonumber(L, -2);
            t.Rotation.z = lua_tonumber(L, -1);
            lua_getfield(L, 3, "Scale");
            lua_getfield(L, -1, "x");
            lua_getfield(L, -2, "y");
            lua_getfield(L, -3, "z");
            t.Scale.x = lua_tonumber(L, -3);
            t.Scale.y = lua_tonumber(L, -2);
            t.Scale.z = lua_tonumber(L, -1);
        }
    }
    return 0;
}

int nimo_luafn_EntityDestroy(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 1);
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "id");
    lua_getfield(L, 1, "scene");
    if((lua_islightuserdata(L, -2) && lua_islightuserdata(L, -1)))
    {
        nimo::GUID* id = (nimo::GUID*)lua_touserdata(L, -2);
        nimo::Scene* scene = (nimo::Scene*)lua_touserdata(L, -1);
        scene->RequestEntityDestruction(scene->GetEntity(*id));
    }
    return 0;
}
int nimo_luafn_EntityInstantiate(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 1);
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "id");
    lua_getfield(L, 1, "assetType");
    if((lua_isinteger(L, -1) && lua_isstring(L, -2)))
    {
        nimo::GUID id(lua_tostring(L, -2));
        nimo::AssetType assetType = (nimo::AssetType)lua_tointeger(L, -1);
        nimo::GUID createdEntityId = nimo::AssetManager::Get<nimo::Prefab>(id)->Create(nimo::SceneManager::GetActiveScene());
        // Push entity table with id and scene
        lua_newtable(L);
        lua_pushlightuserdata(L, (void*)&nimo::SceneManager::GetActiveScene()->GetEntity(createdEntityId).GetComponent<nimo::IDComponent>().Id);
        lua_setfield(L, -2, "id");
        lua_pushlightuserdata(L, (void*)nimo::SceneManager::GetActiveScene().get());
        lua_setfield(L, -2, "scene");
    }
    return 1;
}

int nimo_luafn_EntitySetActive(lua_State* L)
{
    // discard any extra arguments passed in
    lua_settop(L, 2);
    luaL_checktype(L, 1, LUA_TTABLE);
    bool active = lua_toboolean(L, 2);
    lua_getfield(L, 1, "id");
    lua_getfield(L, 1, "scene");
    if((lua_islightuserdata(L, -2) && lua_islightuserdata(L, -1)))
    {
        nimo::GUID* id = (nimo::GUID*)lua_touserdata(L, -2);
        nimo::Scene* scene = (nimo::Scene*)lua_touserdata(L, -1);
        auto e = scene->GetEntity(*id);
        bool prevActive = e.GetComponent<nimo::ActiveComponent>().active;
        if(prevActive != active)
        {
            scene->SetEntityActive(e, active);
        }
    }
    return 0;
}
