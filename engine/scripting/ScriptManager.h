#pragma once
#include "Script.h"
#include "ScriptInstance.h"
#include "scene/Scene.h"
#include <memory>

struct lua_State;
namespace nimo
{
class ScriptManager
{
public:
	static void Initialize();
	static void Cleanup();
    static ScriptInstance CreateInstance(std::shared_ptr<Script> source, const GUID& owner, std::shared_ptr<Scene> scene);
    static void OnCreate(const ScriptInstance& instance);
    static void OnUpdate(const ScriptInstance& instance, float deltaTime);

private: 
    static lua_State* L;
};
} // namespace nimo
