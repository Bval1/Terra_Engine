#pragma once
#include "Terra/Renderer/UniformBuffer.h"
#include "DirectXContext.h"

namespace Terra {
	class DirectXConstantBuffer : public UniformBuffer
	{
	public:
		DirectXConstantBuffer(uint32_t size, uint32_t slot = 0);
		DirectXConstantBuffer(const void* data, uint32_t size, uint32_t slot, ConstantBufferType cbType);
		virtual ~DirectXConstantBuffer() = default;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset) override;
		virtual void Bind(uint32_t numBuffers = 0u) override;
		virtual void Update(const void* data, uint32_t size) override;
		virtual ConstantBufferType GetType() override { return m_type; }
	private:
		DXGIInfoManager& GetInfoManager(DirectXContext& gfx) { return gfx.infoManager; }
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
		D3D11_BUFFER_DESC m_ConstantBufferDesc;
		ConstantBufferType m_type;
		uint32_t m_slot = 0u;
	};
}