#pragma once

#include "Terra/Core/Core.h"
#include "Terra/Events/Event.h"
#include "Terra/Core/Timestep.h"

namespace Terra {

	class Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		
		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
