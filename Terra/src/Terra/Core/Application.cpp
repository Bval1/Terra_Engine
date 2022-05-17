#include "Terrapch.h"
#include "Application.h"
#include "Input.h"
#include <chrono>

#ifdef DIRECTX
//#define IMGUIOFF
#endif
namespace Terra {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	// Holds a singleton to our application
	Application* Application::s_Instance = nullptr;


	Application::Application(const std::wstring& name, ApplicationCommandLineArgs args)
		: m_CommandLineArgs(args)
	{
		TERRA_PROFILE_FUNCTION();

		TERRA_CORE_ASSERT(!s_Instance, "Application already exists!"); 
		s_Instance = this;


		m_Window = std::unique_ptr<Window>(Window::CreateWin32Window(WindowProps(name)));
	
	
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		Renderer::Init();
		m_Window->SetVSync(false);
#ifndef IMGUIOFF
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);	
#endif
	}

	Application::~Application()
	{
		TERRA_PROFILE_FUNCTION();
	}

	void Application::PushLayer(Layer* layer)
	{
		TERRA_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		TERRA_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		TERRA_PROFILE_FUNCTION();

		// Use event dispatcher to assign apporiate events to functions
		EventDispatcher dispatcher(e);

		// calls OnWindowClose() if event is of type WindowCloseEvent
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowsResize));

		//TERRA_CORE_TRACE("{0}", e);	

		// Go backwards through layerstack, break when event is handled
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		TERRA_PROFILE_FUNCTION();
		while (m_Running)
		{
#ifndef DIRECTX
			float time = (float)glfwGetTime();		// or windows QueryPerformanceCounter()
			Timestep timestep = time - m_LastFrameTime2;
			m_LastFrameTime2 = time;
#else
			auto time = std::chrono::steady_clock::now();			
			Timestep timestep = std::chrono::duration<float>(time - m_LastFrameTime).count();
			m_LastFrameTime = time;
#endif
			//TERRA_TRACE("{0} ms frametime", timestep.GetMilliseconds());

			if (!m_Minimized)						// stop rendering if window is minimized
			{
				{
					TERRA_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)			 // submit things for rendering
						layer->OnUpdate(timestep);
				}
		
#ifndef IMGUIOFF		
				m_ImGuiLayer->Begin();
				{
					TERRA_PROFILE_SCOPE("LayerStack OnImGuiRender");
					for (Layer* layer : m_LayerStack)			// call imgui render for all layers
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
#endif
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowsResize(WindowResizeEvent& e)
	{
		TERRA_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)		// window is minimized
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;	// don't want this event blocked for other layers subscribed to it
	}

	void Application::Close()
	{
		m_Running = false;
	}

}