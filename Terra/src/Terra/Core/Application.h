#pragma once

#include "Core.h"
#include "Window.h"
#include "Terra/Core/LayerStack.h"
#include "Terra/Events/Event.h"
#include "Terra/Events/ApplicationEvent.h"
#include "Terra/ImGui/ImGuiLayer.h"

#include "Terra/Core/Timestep.h"
#include "Terra/Renderer/Renderer.h"

namespace Terra {

	

	struct ApplicationCommandLineArgs
	{
#if 1
		int Count = 0;
		char** Args = nullptr;
		const char* operator[](int index) const
		{
			SB_CORE_ASSERT(index < Count);
			return Args[index];
		}	
#else
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		LPSTR lpCmdLine;
		int nCmdShow;
#endif
	};

	// Platform independent singleton
	class Application
	{
	public:
		Application(const std::wstring& name = L"Terra App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();
		
		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
		inline Window& GetWindow() { return *m_Window; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowsResize(WindowResizeEvent& e);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		std::chrono::steady_clock::time_point m_LastFrameTime;
		float m_LastFrameTime2;
		

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT (Sandboxapp)
	Application* CreateApplication(ApplicationCommandLineArgs args);

}
