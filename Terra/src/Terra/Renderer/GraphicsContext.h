#pragma once

namespace Terra {
	
	// An interface implemented per API
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	};
}