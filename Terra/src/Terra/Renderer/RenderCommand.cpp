#include "Terrapch.h"
#include "RenderCommand.h"


#include "Platform/DirectX/DirectXRendererAPI.h"

namespace Terra {

	// In the future will be able to select more APIs
#ifndef DIRECTX
	
#else
	RendererAPI* RenderCommand::s_RendererAPI = new DirectXRendererAPI;
#endif

}