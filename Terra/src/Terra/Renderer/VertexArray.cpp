#include "Terrapch.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/DirectX/DirectXVertexArray.h"

namespace Terra {
	Ref<VertexArray> VertexArray::Create()	// To do: replace raw pointer with Refs
	{
		
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::DirectX: return std::make_shared<DirectXVertexArray>();  // TODO DX VA
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}