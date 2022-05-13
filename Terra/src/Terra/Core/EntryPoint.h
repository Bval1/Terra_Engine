#pragma once
#include "Terra/Core/Core.h"
#ifdef SB_PLATFORM_WINDOWS

extern Terra::Application* Terra::CreateApplication(ApplicationCommandLineArgs args);  // extern found in SandboxApp.cpp
#if 1

int main(int argc, char** argv)		// argc, argv for win32 api purposes
{
	Terra::Log::Init();

	SB_PROFILE_BEGIN_SESSION("Startup", "TerraProfile-Startup.json");
	auto app = Terra::CreateApplication({ argc, argv });	// creates app by calling createapplication
	SB_PROFILE_END_SESSION();
	
	SB_PROFILE_BEGIN_SESSION("Runtime", "TerraProfile-Runtime.json");
	app->Run();									// runs the while rue loop in application::run()
	SB_PROFILE_END_SESSION();
	
	SB_PROFILE_BEGIN_SESSION("Shutdown", "TerraProfile-Shutdown.json");
	delete app;
	SB_PROFILE_END_SESSION();
}
#else
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Terra::Log::Init();

	SB_PROFILE_BEGIN_SESSION("Startup", "TerraProfile-Startup.json");
	auto app = Terra::CreateApplication({ hInstance, hPrevInstance, lpCmdLine, nCmdShow });	// creates app by calling createapplication
	SB_PROFILE_END_SESSION();

	SB_PROFILE_BEGIN_SESSION("Runtime", "TerraProfile-Runtime.json");
	app->Run();									// runs the while true loop in application::run()
	SB_PROFILE_END_SESSION();

	SB_PROFILE_BEGIN_SESSION("Shutdown", "TerraProfile-Shutdown.json");
	delete app;
	SB_PROFILE_END_SESSION();
	return -1;
}
#endif

#endif