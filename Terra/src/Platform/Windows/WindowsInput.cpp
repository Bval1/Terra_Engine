#include "Terrapch.h"
#include "Terra/Core/Input.h"
#include "Terra/Core/Application.h"
#include "Terra/Renderer/RendererAPI.h"

// Implements functions from Input.h, compiled only on Windows
namespace Terra {

	bool Input::IsKeyPressed(int keycode)
	{

		// If the high-order bit is 1, the key is down; otherwise, it is up. If the low-order bit is 1, the key is toggled
		auto state = GetKeyState(keycode);
		short result = state & 0x8000;
		return result;

	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto state = GetKeyState(button);
		short result = state & 0x8000;
		return result;
	}

	std::pair<int, int> Input::GetMousePosition()
	{
		
		auto window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(window, &pos);
		return { pos.x, pos.y };

		TERRA_ASSERT("Unknown RendererAPI!");
		return  { 0, 0 };
	}

	int Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	int Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}