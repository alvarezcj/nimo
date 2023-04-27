#pragma once
#include "Script.h"
#include "ScriptInstance.h"
#include <memory>

struct lua_State;
namespace nimo
{
class ScriptManager
{
public:
	static void Initialize();
	static void Cleanup();
    static ScriptInstance CreateInstance(std::shared_ptr<Script> source);
    static void OnCreate(const ScriptInstance& instance);

private: 
    static lua_State* L;
};
} // namespace nimo
