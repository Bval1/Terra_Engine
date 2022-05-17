#include "Terrapch.h"
#include "DirectXBuffer.h"

Terra::DirectXVertexBuffer::DirectXVertexBuffer(float* vertices, uint32_t size)
	:m_vertices(vertices), m_size(size)
{
}

Terra::DirectXVertexBuffer::DirectXVertexBuffer(uint32_t size)
	:m_size(size)
{
}

void Terra::DirectXVertexBuffer::Bind() const
{
	// Bind VB
	const UINT offset = 0u;
	DirectXRendererAPI::GetDeviceContext()->IASetVertexBuffers(0u, m_numOfBuffers, pVertexBuffer.GetAddressOf(), &m_Stride, &offset);
}

void Terra::DirectXVertexBuffer::Unbind() const
{
}

void Terra::DirectXVertexBuffer::SetData(const void* data, uint32_t size)
{
	TERRA_ASSERT(DirectXRendererAPI::GetDXGraphicsContext(), "Graphics Context is null!");
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT(size);
	m_Stride = m_Layout.GetStride();
	bd.StructureByteStride = m_Stride;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;
	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer));
}

void Terra::DirectXVertexBuffer::SetLayout(const BufferLayout& layout)
{
	m_Layout = layout;
}

Terra::DirectXIndexBuffer::DirectXIndexBuffer(uint32_t* indices, uint32_t count)
	: m_Count(count)
{
	TERRA_ASSERT(DirectXRendererAPI::GetDXGraphicsContext(), "Graphics Context is null!");
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	UINT size = count * sizeof(*indices);
	ibd.ByteWidth = size;
	ibd.StructureByteStride = sizeof(uint32_t);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateBuffer(&ibd, &isd, &pIndexBuffer));
}

Terra::DirectXIndexBuffer::~DirectXIndexBuffer()
{
	
}

void Terra::DirectXIndexBuffer::Bind() const
{
	DirectXRendererAPI::GetDeviceContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

void Terra::DirectXIndexBuffer::Unbind() const
{
}
