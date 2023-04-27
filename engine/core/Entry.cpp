#include "Entry.h"
#include "core/Log.h"
#include "scripting/ScriptManager.h"
#include "assets/AssetManager.h"

int main(int argc, char** argv)
{
    nimo::Log::Initialize();
	auto app = CreateApplication();
	if(app == nullptr) 
	{
		NIMO_CRITICAL("Client application not created!");
		return 1;
	}
	app->Run();
	delete app;
    nimo::ScriptManager::Cleanup();
	nimo::AssetManager::Cleanup();
    nimo::Log::Cleanup();
    return 0;
}