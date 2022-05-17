#include "Terrapch.h"
#include "Win32Window.h"
#include "Terra/Core/Application.h"
#include "Platform/DirectX/DirectXContext.h"
#include <backends\imgui_impl_dx11.h>
#include <backends\imgui_impl_win32.h>
//#define IMGUI_OFF

#ifndef IMGUI_OFF
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // !IMGUI_OFF


Terra::WindowClass Terra::WindowClass::s_wndClass;

Terra::Window* Terra::Window::CreateWin32Window(const WindowProps& props)
{
	return new Win32Window(props);
}

Terra::WindowClass::WindowClass() noexcept
	:hInst(::GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = Win32Window::HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	//wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	//wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	::RegisterClassEx(&wc);
}

Terra::WindowClass::~WindowClass()
{
	::UnregisterClass(GetName(), GetInstance());
}

Terra::Win32Window::Win32Window(const WindowProps& props)
{
	Init(props);
}

Terra::Win32Window::~Win32Window()
{

#ifndef IMGUI_OFF
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
	Shutdown();
}

void Terra::Win32Window::Init(const WindowProps& props)
{
#ifndef IMGUI_OFF
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

#endif

	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;
	m_Data.RawMouseInput = props.RawMouseInput;
	m_Data.Vsync = props.Vsync;

	//CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
	RECT wr;
	wr.left = 0;
	wr.right = GetWidth() + wr.left;
	wr.top = 0;
	wr.bottom = GetHeight() + wr.top;
	if (!AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU, FALSE))
		throw HrException(__LINE__, __FILE__, GetLastError());

	// create window & get hWnd
	hWnd = CreateWindow(WindowClass::GetName(), GetTitle(), WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);

	if (hWnd == nullptr)
		throw HrException(__LINE__, __FILE__, GetLastError());

	// show because newly created windows start off as hidden
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	// create graphics context
	m_Context = std::make_shared<DirectXContext>(hWnd, m_Data.Width, m_Data.Height);
	m_Context->Init();

	// register mouse raw input device
	::RAWINPUTDEVICE rid;
	memset(&rid, 0, sizeof(rid));
	rid.usUsagePage = 0x01;		// mouse page
	rid.usUsage = 0x02;			// mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (!::RegisterRawInputDevices(&rid, 1, sizeof(rid)))
		throw WND_LAST_EXCEPT();
}

void Terra::Win32Window::Shutdown()
{
	::DestroyWindow(hWnd);
}

void Terra::Win32Window::OnUpdate()
{

	m_Context->SwapBuffers();
	if (const auto ecode = Win32Window::ProcessMessages())
	{
		// if return optional has value, means we're quitting so return exit code
		return;
	}
}

LRESULT Terra::Win32Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in when window is created to store our window class pointer
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Win32Window* const pWnd = static_cast<Win32Window*>(pCreate->lpCreateParams);

		// set WinAPI-managed user data to store ptr to our window class
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// set message proc to normal (non-setup) handler now that setup is finished
		::SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Win32Window::HandleMsgThunk));

		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	// if we get a message before the WM_NCCREATE message, handle with default handler
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Terra::Win32Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Win32Window* const pWnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler (our member function)
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Terra::Win32Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
#ifndef IMGUI_OFF

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	//const auto imio = ImGui::GetIO();

#endif
	switch (msg)
	{
		// We don't want the DefProc to handle this message because we want our destructor to destroy window,
		// so return 0 instead of break
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		WindowCloseEvent event;
		m_Data.EventCallback(event);
		return 0;
	}

	case WM_ACTIVATE:
		// confine/free cursor on window to foreground/background if cursor disabled
		if (!m_CursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				TERRA_TRACE("activate => confine cursor");
				RECT rect;
				::GetClientRect(hWnd, &rect);
				::MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
				::ClipCursor(&rect);
			}
			else
			{
				TERRA_TRACE("activate => free cursor");
				::ClipCursor(nullptr);
			}
		}
		break;

	case WM_SIZING:
	{
		RECT* ws = (RECT*)lParam;
		m_Data.Width = (uint32_t)(ws->right - ws->left - 16);
		m_Data.Height = (uint32_t)(ws->bottom - ws->top - 39);
		WindowResizeEvent event(m_Data.Width, m_Data.Height);
		m_Data.EventCallback(event);
		break;
	}

	//******* Keyboard Events *************//
	case WM_KEYDOWN:
	{
	case WM_SYSKEYDOWN:		// for keystroke info (not case sensitive)	
		static int count = 0;
		if (lParam & 0x40000000)
			count++;
		else
			count = 0;
		KeyPressedEvent event((int)wParam, count);
		m_Data.EventCallback(event);
		if (wParam == VK_INSERT)
		{
			m_Data.RawMouseInput = m_Data.RawMouseInput ? false : true;
			if (m_CursorEnabled)
			{
				m_CursorEnabled = false;
				HideCursor();
				break;
			}
			else
			{
				m_CursorEnabled = true;
				ShowCursor();
				break;
			}
		}

		break;
		
	}

	case WM_KEYUP:
	{
	case WM_SYSKEYUP:
		KeyReleasedEvent event((int)wParam);
		m_Data.EventCallback(event);
		break;
	}

	case WM_CHAR:
	{
		KeyTypedEvent event((unsigned int)wParam);
		m_Data.EventCallback(event);
		break;
	}
	
	//******* Mouse Events *************//
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < GetWidth() && pt.y >= 0 && pt.y < GetHeight())
		{
			::SetCapture(hWnd);
			MouseMovedEvent event(pt.x, pt.y);
			m_Data.EventCallback(event);		
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) // can also use mouse.LeftIsPressed()
			{
				MouseMovedEvent event(pt.x, pt.y);
				m_Data.EventCallback(event);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		MouseButtonPressedEvent event((int)wParam);
		m_Data.EventCallback(event);
		break;
	}
	

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		MouseButtonReleasedEvent event((int)wParam);
		m_Data.EventCallback(event);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		MouseScrolledEvent event(0, delta);
		m_Data.EventCallback(event);
		break;
	}
	case WM_MOUSEHWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		MouseScrolledEvent event(delta, 0);
		m_Data.EventCallback(event);
		break;
	}

	// Raw Mouse Messages
	case WM_INPUT:
	{
		if (!m_Data.RawMouseInput)
			break;

		UINT size;

		// fills size with number of bytes required for input data
		if (::GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
		{
			TERRA_WARN("Raw mouse input error");
			break;	
		}

		m_RawBuffer.resize(size);
		// read input data
		if (::GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT, m_RawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
		{
			TERRA_WARN("Raw mouse input error");
			break;
		}

		// process the raw input data
		auto& ri = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			// onrawdelta mouse event
			RawMouseMovedEvent event(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
			m_Data.EventCallback(event);
			TERRA_TRACE("{0}", event.ToString());
		}
		break;	
	}

	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Terra::Win32Window::ProcessMessages()
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return value
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return msg.wParam;
		}
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}

void Terra::Win32Window::SetVSync(bool enabled)
{
	m_Data.Vsync = enabled;
}

void Terra::Win32Window::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0); // showcursor(true) increments 
}

void Terra::Win32Window::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);	// showcursor(false) decrements
}

void Terra::Win32Window::EnableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // ~ inverts bits
}

void Terra::Win32Window::DisableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

const char* Terra::Win32Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}

std::string Terra::Win32Window::HrException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;

	DWORD nMsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	// if length of error string is 0 something went wrong
	if (nMsgLen == 0)
		return "Unidentified error code";

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}