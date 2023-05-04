#include "ScriptManager.h"
#include "lua.hpp"
#include "ScriptUtils.h"
#include "core/Log.h"
#include "core/Application.h"
#include "project/Project.h"
#include "lua_hooks/LuaDebug.h"
#include "lua_hooks/LuaInput.h"
#include "lua_hooks/LuaComponents.h"
#include "lua_hooks/LuaApplication.h"
#include "lua_hooks/LuaWindow.h"
#include "lua_hooks/LuaSceneManagement.h"

lua_State* nimo::ScriptManager::L = nullptr;

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
        lua_pushcfunction(L, nimo_luafn_EntityDestroy);
        lua_setfield(L, -2, "Destroy");
        lua_setfield(L, -2, "Entity");
    }
    // Application
    {
        lua_newtable(L);
        lua_pushcfunction(L, nimo_luafn_ApplicationClose);
        lua_setfield(L, -2, "Close");
        lua_setfield(L, -2, "Application");
    }
    // Time
    {
        lua_newtable(L);
        lua_setfield(L, -2, "Time");
    }
    // Window
    {
        lua_newtable(L);
        lua_pushcfunction(L, nimo_luafn_WindowMaximize);
        lua_setfield(L, -2, "Maximize");
        lua_pushcfunction(L, nimo_luafn_WindowRestore);
        lua_setfield(L, -2, "Restore");
        lua_pushcfunction(L, nimo_luafn_WindowSetTitle);
        lua_setfield(L, -2, "SetTitle");
        lua_pushcfunction(L, nimo_luafn_WindowGetTitle);
        lua_setfield(L, -2, "GetTitle");
        lua_pushcfunction(L, nimo_luafn_WindowGetSize);
        lua_setfield(L, -2, "GetSize");
        lua_setfield(L, -2, "Window");
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
    // Scene management
    {
        lua_newtable(L);
        lua_pushcfunction(L, nimo_luafn_SceneManagementLoadScene);
        lua_setfield(L, -2, "LoadScene");
        lua_setfield(L, -2, "SceneManagement");
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
            // Look for Public table
            if(lua_getfield(L, -1, "Public") == LUA_TTABLE)
            {
                // Iterate over the public fields table
                lua_pushvalue(L, -1);
                lua_pushnil(L);
                while (lua_next(L, -2))
                {
                    lua_pushvalue(L, -2);
                    int type = lua_type(L,-2);
                    const char *key = lua_tostring(L, -1);
                    // const char *value = lua_tostring(L, -2);
                    // NIMO_DEBUG("{}({}) - {}", key, type, value);
                    // Create map with public fields memory representation to serialize/deserialize
                    std::map<std::string, void*> fieldMap;
                    switch(type)
                    {
                    case LUA_TNUMBER:
                        res.fields[key] = std::make_shared<ScriptFieldNumber>(key, lua_tonumber(L, -2));
                        break;
                    case LUA_TSTRING:
                        res.fields[key] = std::make_shared<ScriptFieldString>(key, lua_tostring(L, -2));
                        break;
                    case LUA_TBOOLEAN:
                        res.fields[key] = std::make_shared<ScriptFieldBool>(key, lua_toboolean(L, -2));
                        break;
                    default:
                        break;
                    }
                    lua_pop(L, 2);
                }
                lua_pop(L, 1);
            }
            lua_remove(L, -1);

            // Push entity table with id and scene
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
        }
    }
    ScriptUtils::PrintLuaStack(L);
    return res;
}
void nimo::ScriptManager::ApplyFields(const ScriptInstance& instance)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
    // Look for Public table
    if(lua_getfield(L, -1, "Public") == LUA_TTABLE)
    {
        for(const auto& field : instance.fields)
        {
            switch (field.second->GetType())
            {
            case nimo::ScriptFieldType::Number:
                lua_pushnumber(L, std::static_pointer_cast<nimo::ScriptFieldNumber>(field.second)->value);
                lua_setfield(L, -2, field.first.c_str());
                break;
            case nimo::ScriptFieldType::Boolean:
                lua_pushboolean(L, std::static_pointer_cast<nimo::ScriptFieldBool>(field.second)->value);
                lua_setfield(L, -2, field.first.c_str());
                break;
            case nimo::ScriptFieldType::String:
                lua_pushstring(L, std::static_pointer_cast<nimo::ScriptFieldString>(field.second)->value.c_str());
                lua_setfield(L, -2, field.first.c_str());
                break;
            default:
                break;
            }
        }
    }
    lua_remove(L, -1);
    lua_remove(L, -1);
}
void nimo::ScriptManager::UpdateTime(float deltaTime)
{
    lua_getglobal(L, "nimo");
    lua_getfield(L, -1, "Time");
    lua_pushnumber(L, deltaTime);
    lua_setfield(L, -2, "deltaTime");
    lua_pushnumber(L, Application::Instance().Time());
    lua_setfield(L, -2, "time");
    lua_remove(L, -1);
    lua_remove(L, -1);
}
void nimo::ScriptManager::OnCreate(const ScriptInstance& instance)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
    lua_pushstring(L, "OnCreate");
    lua_gettable(L, -2);
    if(!lua_isnil(L, -1))
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
        lua_pcall(L, 1, 0, 0);
    }
    else
    {
        NIMO_DEBUG("No OnCreate function");
    }
    lua_pop(L, 1);
}
void nimo::ScriptManager::OnUpdate(const ScriptInstance& instance)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
    lua_pushstring(L, "OnUpdate");
    lua_gettable(L, -2);
    if(!lua_isnil(L, -1))
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, instance.stackReference);
        lua_pcall(L, 1, 0, 0);
    }
    lua_pop(L, 1);
}