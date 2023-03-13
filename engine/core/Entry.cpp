#include "Entry.h"
#include "core/Log.h"

int main(int argc, char** argv)
{
    nimo::Log::Initialize();
	auto app = CreateApplication();
	app->Run();
	delete app;
    return 0;
}