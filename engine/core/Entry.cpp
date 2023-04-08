#include "Entry.h"
#include "core/Log.h"

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
    return 0;
}