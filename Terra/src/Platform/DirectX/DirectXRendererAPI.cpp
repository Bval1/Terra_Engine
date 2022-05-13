#include "Terrapch.h"
#include "DirectXRendererAPI.h"
#include "Platform/Windows/Win32Window.h"
#include "Platform/DirectX/DirectXContext.h"
Terra::Ref<Terra::DirectXContext> Terra::DirectXRendererAPI::m_DXContext;
void Terra::DirectXRendererAPI::Init()
{
	SB_PROFILE_FUNCTION();
	m_DXContext = std::dynamic_pointer_cast<DirectXContext>(Application::Get().GetWindow().GetGraphicsContext());
	SB_ASSERT(m_DXContext, "DirectX Context is null!");
}

void Terra::DirectXRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	GetDeviceContext()->RSSetViewports(1u, &vp);
}

void Terra::DirectXRendererAPI::SetClearColor(const glm::vec4& color)
{
	const float clearcolor[] = { color.r, color.g, color.b, color.a };
	GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView().Get(), clearcolor);
}

void Terra::DirectXRendererAPI::Clear()
{
	GetDeviceContext()->ClearDepthStencilView(GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Terra::DirectXRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
{
	INFOMAN(*m_DXContext);
#if 0
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};

	Vertex vertices[] =
	{
		{ -1.0f,-1.0f,-1.0f	 },
		{ 1.0f,-1.0f,-1.0f	 },
		{ -1.0f,1.0f,-1.0f	 },
		{ 1.0f,1.0f,-1.0f	  },
		{ -1.0f,-1.0f,1.0f	 },
		{ 1.0f,-1.0f,1.0f	  },
		{ -1.0f,1.0f,1.0f	 },
		{ 1.0f,1.0f,1.0f	 },
	};
	
	// VertexBuffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT(sizeof(vertices));
	UINT vbStride = UINT(sizeof(Vertex));
	bd.StructureByteStride = vbStride;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_INFO(GetDevice(m_DXContext)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	// Bind VB
	const UINT offset = 0u;
	GetDeviceContext(m_DXContext)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &vbStride, &offset);

	// IndexBuffer
	const unsigned short indices[] =
	{
				0,2,1,    2,3,1,	// front 
				1,3,5,	  3,7,5,	// right 
				2,6,3,	  3,6,7,	// top
				4,5,7,    4,7,6,	// back
				0,4,2,	  2,4,6,	// left
				0,1,4,	  1,5,4		// bottom

	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_INFO(GetDevice(m_DXContext)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	// Bind IB
	GetDeviceContext(m_DXContext)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	
	// Constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	const ConstantBuffer cb =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(0.0f) *
				DirectX::XMMatrixRotationX(-60.0f) *
				DirectX::XMMatrixRotationY(180.0f)*
				DirectX::XMMatrixTranslation(0.0f,0.0f,4.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
			)
		}
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_INFO(GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	// Bind TCB to vertex shader
	GetDeviceContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// Constant buffer for face colors
	struct RGBA
	{
		float r, g, b, a;
	};
	const RGBA facecolors[6] = 
	{	
		{1.0f,0.0f,1.0f},
		{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f},
		{1.0f,1.0f,0.0f},
		{0.0f,1.0f,1.0f},		
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(facecolors);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &facecolors;
	GFX_THROW_INFO(GetDevice()->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));
	// bind CB to pixel shader
	GetDeviceContext()->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());
	// Vertex Shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	auto VSpath = L"assets/shaders/D3D11/VS.cso";
	Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(VSpath, &pByteCodeBlob));
	GFX_THROW_INFO(GetDevice(m_DXContext)->CreateVertexShader(
		pByteCodeBlob->GetBufferPointer(), pByteCodeBlob->GetBufferSize(), nullptr, &pVertexShader));
	// Bind VS
	GetDeviceContext(m_DXContext)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	
	// Pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	auto PSpath = L"assets/shaders/D3D11/PS.cso";
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(PSpath, &pBlob));
	GFX_THROW_INFO(GetDevice(m_DXContext)->CreatePixelShader(
		pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	// Bind PS
	GetDeviceContext(m_DXContext)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	
	// Input Layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	auto pVSBytecode = pByteCodeBlob.Get();
	GFX_THROW_INFO(GetDevice(m_DXContext)->CreateInputLayout(
		ied.data(), (UINT)ied.size(), pVSBytecode->GetBufferPointer(), pVSBytecode->GetBufferSize(), 
		&pInputLayout));
	// Bind Layout
	GetDeviceContext(m_DXContext)->IASetInputLayout(pInputLayout.Get());
#endif

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
