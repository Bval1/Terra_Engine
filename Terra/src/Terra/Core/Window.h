#pragma once

#include "Terra/Core/Core.h"
#include "Terra/Events/Event.h"
#include "Terra/Renderer/GraphicsContext.h"

namespace Terra {

	struct WindowProps
	{
		std::wstring Title;
		uint32_t Width;
		uint32_t Height;
		bool RawMouseInput;
		bool Vsync;

		WindowProps(const std::wstring& title = L"Terra Engine",
			uint32_t width = 1280, uint32_t height = 720, 
			bool rawMouseInput = false, bool vsync = false)
			: Title(title), Width(width), Height(height), RawMouseInput(rawMouseInput), Vsync(vsync)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual Ref<GraphicsContext> GetGraphicsContext() = 0;

		// Gets implemented per platform
		static Window* CreateWin32Window(const WindowProps& props = WindowProps());
	};

}
