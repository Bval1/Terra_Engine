#pragma once

#include "Terra/Core/Core.h"

// Static declarations of functions here, implement in seperate cpp files that get compiled per platform
// Linker will link appropriate defintions to these declarations

namespace Terra {

	class Input
	{
	public:
	
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		
		// Alternative way to get mouse position
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}