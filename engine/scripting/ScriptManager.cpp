#include "ScriptManager.h"
#include "lua.hpp"
#include "ScriptUtils.h"
#include "core/Log.h"
#include "project/Project.h"
#include "scene/Scene.h"
#include "scene/Components.h"
#include "input/Input.h"

lua_State* nimo::ScriptManager::L = nullptr;
const nimo::ScriptInstance* nimo::ScriptManager::processingInstance = nullptr;

int nimo_luafn_DebugLog(lua_State* L)
{
    std::string trace = lua_tostring(L, 1);
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

void nimo::ScriptManager::Initialize()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    // Set asset folder as package path

    std::string code = "package.path = [[" + Project::GetActiveProject()->GetAssetsFolderPath().string() + "\\?.lua]]";
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
        lua_pushcfunction(L, nimo_luafn_HasEntityComponent);
        lua_setfield(L, -2, "HasComponent");
        lua_pushcfunction(L, nimo_luafn_GetEntityComponent);
        lua_setfield(L, -2, "GetComponent");
        lua_pushcfunction(L, nimo_luafn_SetEntityComponent);
        lua_setfield(L, -2, "SetComponent");
        lua_setfield(L, -2, "Entity");
    }
    // Input
    {
        lua_newtable(L);
        lua_pushcfunction(L, nimo_luafn_InputGetKey);
        lua_setfield(L, -2, "GetKey");
        lua_pushcfunction(L, nimo_luafn_InputGetMouseButton);
        lua_setfield(L, -2, "GetMouseButton");
        lua_pushcfunction(L, nimo_luafn_InputGetMousePosition);
        lua_setfield(L, -2, "GetMousePosition");
        lua_setfield(L, -2, "Input");
    }
    // KeyCode
    {
        const char* keycodelua = R"(
        local KeyCode =
        {
            Space = 32,
            Apostrophe = 39, 
            Comma = 44, 
            Minus = 45, 
            Period = 46, 
            Slash = 47, 

            D0 = 48, 
            D1 = 49, 
            D2 = 50, 
            D3 = 51, 
            D4 = 52, 
            D5 = 53, 
            D6 = 54, 
            D7 = 55, 
            D8 = 56, 
            D9 = 57, 

            Semicolon = 59, 
            Equal = 61, 

            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,

            LeftBracket = 91,  
            Backslash = 92,  
            RightBracket = 93,  
            GraveAccent = 96,  

            World1 = 161, 
            World2 = 162, 

            
            Escape = 256,
            Enter = 257,
            Tab = 258,
            Backspace = 259,
            Insert = 260,
            Delete = 261,
            Right = 262,
            Left = 263,
            Down = 264,
            Up = 265,
            PageUp = 266,
            PageDown = 267,
            Home = 268,
            End = 269,
            CapsLock = 280,
            ScrollLock = 281,
            NumLock = 282,
            PrintScreen = 283,
            Pause = 284,
            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,

            
            KP0 = 320,
            KP1 = 321,
            KP2 = 322,
            KP3 = 323,
            KP4 = 324,
            KP5 = 325,
            KP6 = 326,
            KP7 = 327,
            KP8 = 328,
            KP9 = 329,
            KPDecimal = 330,
            KPDivide = 331,
            KPMultiply = 332,
            KPSubtract = 333,
            KPAdd = 334,
            KPEnter = 335,
            KPEqual = 336,

            LeftShift = 340,
            LeftControl = 341,
            LeftAlt = 342,
            LeftSuper = 343,
            RightShift = 344,
            RightControl = 345,
            RightAlt = 346,
            RightSuper = 347,
            Menu = 348
        }
        return KeyCode)";
        luaL_dostring(L, keycodelua);
        lua_setfield(L, -2, "KeyCode");
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
        lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
        lua_pushnumber(L, deltaTime);
        lua_pcall(L, 2, 0, 0);
    }
    lua_pop(L, 1);
    processingInstance = nullptr;
}