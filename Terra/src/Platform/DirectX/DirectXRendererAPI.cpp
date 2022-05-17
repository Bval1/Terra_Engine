#include "Terrapch.h"
#include "DirectXRendererAPI.h"
#include "Platform/Windows/Win32Window.h"
#include "Platform/DirectX/DirectXContext.h"
Terra::Ref<Terra::DirectXContext> Terra::DirectXRendererAPI::m_DXContext;
void Terra::DirectXRendererAPI::Init()
{
	TERRA_PROFILE_FUNCTION();
	m_DXContext = std::dynamic_pointer_cast<DirectXContext>(Application::Get().GetWindow().GetGraphicsContext());
	TERRA_ASSERT(m_DXContext, "DirectX Context is null!");
}

void Terra::DirectXRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = (float)x;
	vp.TopLeftY = (float)y;
	GetDeviceContext()->RSSetViewports(1u, &vp);
}

void Terra::DirectXRendererAPI::SetClearColor(const DirectX::XMFLOAT4& color)
{
	const float clearcolor[] = { color.x, color.y, color.z, color.w };
	GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView().Get(), clearcolor);
}

void Terra::DirectXRendererAPI::Clear()
{
	GetDeviceContext()->ClearDepthStencilView(GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Terra::DirectXRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
{
	INFOMAN(*m_DXContext);

	// Bind Render Target
	GetDeviceContext()->OMSetRenderTargets(1u, GetRenderTargetView().GetAddressOf(), GetDepthStencilView().Get());
	// Bind Topology
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Set Viewport
	SetViewPort(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());	
	// Draw
	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	GFX_THROW_INFO_ONLY(GetDeviceContext()->DrawIndexed((UINT)count, 0u, 0u));
}

void Terra::DirectXRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
{
}

void Terra::DirectXRendererAPI::SetLineWidth(float width)
{
}
