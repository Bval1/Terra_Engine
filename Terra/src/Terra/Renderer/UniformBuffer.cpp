#include "Terrapch.h"
#include "UniformBuffer.h"
#include "Terra/Renderer/Renderer.h"
#include "Platform/DirectX/DirectXConstantBuffer.h"

namespace Terra {
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::DirectX: return CreateRef<DirectXConstantBuffer>(size, binding);
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::Create(const void* data, uint32_t size, uint32_t slot,
		ConstantBufferType cbType)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::DirectX: return CreateRef<DirectXConstantBuffer>(data, size, slot, cbType);
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}