#include "Terrapch.h"
#include "RendererAPI.h"

namespace Terra {
#ifdef DIRECTX
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::DirectX;
#else
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif
}