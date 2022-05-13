#pragma once

#include "Terra/Core/Layer.h"
#include "Terra/Events/ApplicationEvent.h"
#include "Terra/Events/KeyEvent.h"
#include "Terra/Events/MouseEvent.h"

namespace Terra {

	class ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;		
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		// Activate to have ImGui layers to block mouse and keyboard events
		void SetBlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}