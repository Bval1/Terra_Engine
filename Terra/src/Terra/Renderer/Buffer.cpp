#include "Terrapch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/DirectX/DirectXBuffer.h"

namespace Terra
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		// Decide which API you're using
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::DirectX: return std::make_shared<DirectXVertexBuffer>(size);
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::DirectX: return std::make_shared<DirectXVertexBuffer>(vertices, size);
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::DirectX: return std::make_shared<DirectXIndexBuffer>(indices, count);
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}
}