#pragma once
#include "Terra/Core/Window.h"
#include "Terra/Core/Exception.h"
#include "Terra/Renderer/GraphicsContext.h"
#include "Win32Config.h"
#include <sstream>

namespace Terra
{
	class WindowClass
	{
	public:
		static const wchar_t* GetName() { return s_wndClass.wndClassName; }
		static HINSTANCE GetInstance() { return s_wndClass.hInst; }
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"DirectX Window Class";
		static WindowClass s_wndClass;
		HINSTANCE hInst;
	};

	class Win32Window : public Window
	{
		friend class WindowClass;
	public:
		class HrException : public TerraException
		{
		public:
			HrException(int line, const char* file, HRESULT hr)
				: TerraException(line, file), hr(hr) {}
			
			const char* what() const noexcept override;
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
			
			virtual const char* GetType() const noexcept override { return "Window Exception"; }
			std::string GetErrorDescription() const noexcept { return TranslateErrorCode(hr); }
			HRESULT GetErrorCode() const noexcept { return hr; }
		private:
			HRESULT hr;
		};
		class NoGfxException : public TerraException
		{
		public:
			using TerraException::TerraException;
			const char* GetType() const noexcept override { return "Window Exception [No Graphics]"; }
		};
	public:
		Win32Window(const WindowProps& props);
		~Win32Window();
		Win32Window(const Win32Window&) = delete;
		Win32Window& operator=(const Win32Window&) = delete;
		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		// Window attributes
		std::optional<int> ProcessMessages();
		inline virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override { return m_Data.Vsync;} 
		virtual void* GetNativeWindow() const override { return hWnd; }
		virtual Ref<GraphicsContext> GetGraphicsContext() override { return m_Context; }
		
		void ShowCursor() noexcept;
		void HideCursor() noexcept;
		void EnableImGuiMouse() noexcept;
		void DisableImGuiMouse() noexcept;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		const wchar_t* GetTitle() noexcept { return m_Data.Title.c_str(); }
		// use these static functions to register member function HandleMsg as a windows procedures
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	
	private:
		struct WindowData
		{
			std::wstring Title;
			uint32_t Width, Height;
			bool RawMouseInput;
			bool Vsync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		bool m_CursorEnabled = true;
		HWND hWnd;
		Ref<GraphicsContext> m_Context;
		std::vector<char> m_RawBuffer;
	};
}