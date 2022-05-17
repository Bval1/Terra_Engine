#pragma once
#include "Terra/Core/Core.h"
#ifdef TERRA_PLATFORM_WINDOWS

extern Terra::Application* Terra::CreateApplication(ApplicationCommandLineArgs args);  // extern found in SandboxApp.cpp
#ifndef NDEBUG		

int main(int argc, char** argv)		// argc, argv for win32 api purposes
{
	Terra::Log::Init();

	TERRA_PROFILE_BEGIN_SESSION("Startup", "TerraProfile-Startup.json");
	auto app = Terra::CreateApplication({ argc, argv });	// creates app by calling createapplication
	TERRA_PROFILE_END_SESSION();
	
	TERRA_PROFILE_BEGIN_SESSION("Runtime", "TerraProfile-Runtime.json");
	app->Run();									// runs the while rue loop in application::run()
	TERRA_PROFILE_END_SESSION();
	
	TERRA_PROFILE_BEGIN_SESSION("Shutdown", "TerraProfile-Shutdown.json");
	delete app;
	TERRA_PROFILE_END_SESSION();
}
#else	// Don't show console window on release mode
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Terra::Log::Init();

	TERRA_PROFILE_BEGIN_SESSION("Startup", "TerraProfile-Startup.json");
	auto app = Terra::CreateApplication({ hInstance, hPrevInstance, lpCmdLine, nCmdShow });	// creates app by calling createapplication
	TERRA_PROFILE_END_SESSION();

	TERRA_PROFILE_BEGIN_SESSION("Runtime", "TerraProfile-Runtime.json");
	app->Run();									// runs the while true loop in application::run()
	TERRA_PROFILE_END_SESSION();

	TERRA_PROFILE_BEGIN_SESSION("Shutdown", "TerraProfile-Shutdown.json");
	delete app;
	TERRA_PROFILE_END_SESSION();
	return -1;
}
#endif

#endif