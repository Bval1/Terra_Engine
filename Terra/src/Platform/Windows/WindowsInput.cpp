#include "Terrapch.h"
#include "Terra/Core/Input.h"
#include "Terra/Core/Application.h"
#include "Terra/Renderer/RendererAPI.h"
#include <GLFW/glfw3.h>

// Implements functions from Input.h, compiled only on Windows
namespace Terra {

	bool Input::IsKeyPressed(int keycode)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
				auto state = glfwGetKey(window, keycode);
				return state == GLFW_PRESS || state == GLFW_REPEAT;
			}	
			case RendererAPI::API::DirectX:
			{
				// If the high-order bit is 1, the key is down; otherwise, it is up. If the low-order bit is 1, the key is toggled
				auto state = GetKeyState(keycode);
				short result = state & 0x8000;
				return result;
			}
		}
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
				auto state = glfwGetMouseButton(window, button);
				return state == GLFW_PRESS;
			}
			case RendererAPI::API::DirectX:
			{
				auto state = GetKeyState(button);
				short result = state & 0x8000;
				return result;
			}
		}
	
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				return { (float)xpos, (float)ypos };
			}
			case RendererAPI::API::DirectX:
			{
				auto window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());
				POINT pos;
				GetCursorPos(&pos);
				ScreenToClient(window, &pos);
				return { (float)pos.x, (float)pos.y };
			}
		}

		SB_ASSERT("Unknown RendererAPI!");
		return  { 0, 0 };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}