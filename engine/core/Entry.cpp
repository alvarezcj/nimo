#include "Entry.h"
#include "core/Log.h"
#include "scripting/ScriptManager.h"
#include "assets/AssetManager.h"
#include "scene/SceneManager.h"
#include "input/Input.h"

int main(int argc, char** argv)
{
    nimo::Log::Initialize();
    nimo::Input::Initialize();
	auto app = CreateApplication();
	if(app == nullptr) 
	{
		NIMO_CRITICAL("Client application not created!");
		return 1;
	}
	app->Run();
	delete app;
    nimo::ScriptManager::Cleanup();
	nimo::SceneManager::SetActiveScene({});
	nimo::AssetManager::UnloadUnused();
	nimo::AssetManager::Cleanup();
    nimo::Input::Cleanup();
    //nimo::Log::Cleanup();
    return 0;
}