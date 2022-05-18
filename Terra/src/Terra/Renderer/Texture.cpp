#include "Terrapch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/DirectX/DirectXTexture2D.h"

namespace Terra {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::DirectX: return CreateRef<DirectXTexture2D>(width, height);
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path, uint32_t slot)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::DirectX: return CreateRef<DirectXTexture2D>(path, slot);
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}