#include "Terrapch.h"
#include "DirectXConstantBuffer.h"
#include "DirectXRendererAPI.h"

Terra::DirectXConstantBuffer::DirectXConstantBuffer(uint32_t size, uint32_t slot)
{
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	m_ConstantBufferDesc = {};
	m_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_ConstantBufferDesc.MiscFlags = 0u;
	m_ConstantBufferDesc.ByteWidth = size;
	m_ConstantBufferDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateBuffer(&m_ConstantBufferDesc, nullptr, &pConstantBuffer));
}

Terra::DirectXConstantBuffer::DirectXConstantBuffer(const void* data, uint32_t size, uint32_t slot, ConstantBufferType cbType)
	: m_type(cbType), m_slot(slot)
{

	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	m_ConstantBufferDesc = {};
	m_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_ConstantBufferDesc.MiscFlags = 0u;
	m_ConstantBufferDesc.ByteWidth = size;
	m_ConstantBufferDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = data;
	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateBuffer(&m_ConstantBufferDesc, &csd, &pConstantBuffer));
}



void Terra::DirectXConstantBuffer::Bind(uint32_t numBuffers)
{
	switch (m_type)
	{
	case ConstantBufferType::None:
		SB_ASSERT("Must select a valid constant buffer type!");
		break;
	case ConstantBufferType::Vertex:
		DirectXRendererAPI::GetDeviceContext()->VSSetConstantBuffers(m_slot, numBuffers, pConstantBuffer.GetAddressOf());
		break;
	case ConstantBufferType::Pixel:
		DirectXRendererAPI::GetDeviceContext()->PSSetConstantBuffers(m_slot, numBuffers, pConstantBuffer.GetAddressOf());
		break;
	}
	
}

void Terra::DirectXConstantBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
{
	Update(data, size);
}

void Terra::DirectXConstantBuffer::Update(const void* data, uint32_t size)
{
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	D3D11_MAPPED_SUBRESOURCE msr;
	GFX_THROW_INFO(DirectXRendererAPI::GetDeviceContext()->Map(pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &msr));
	memcpy(msr.pData, data, size);
	DirectXRendererAPI::GetDeviceContext()->Unmap(pConstantBuffer.Get(), 0);
}
