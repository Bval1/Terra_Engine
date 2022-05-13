#include "Terrapch.h"
#include "DirectXContext.h"
#include <sstream>
#include "DirectXThrowMacros.h"


// when compiler hits this pragma it sets linker setting for us 
#pragma comment(lib, "d3d11.lib")
// link this library to be able to load cso shader binary files
#pragma comment (lib, "D3DCompiler.lib")

Microsoft::WRL::ComPtr<ID3D11DeviceContext>  Terra::DirectXContext::pDeviceContext = NULL;
Microsoft::WRL::ComPtr<ID3D11Device> Terra::DirectXContext::pDevice = NULL;
Microsoft::WRL::ComPtr<IDXGISwapChain> Terra::DirectXContext::pSwapChain = NULL;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Terra::DirectXContext::pRenderTarget = NULL;
Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Terra::DirectXContext::pDepthStencilView = NULL;

Terra::DirectXContext::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	: SBException(line, file), m_Hr(hr)
{
	for (const auto& m : infoMsgs)
	{
		m_Info += m;
		m_Info.push_back('\n');
	}
	// remove final newline if exists
	if (!m_Info.empty())
		m_Info.pop_back();
}

const char* Terra::DirectXContext::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!m_Info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();

}

std::string Terra::DirectXContext::HrException::GetErrorDescription() const noexcept
{
	char* pMsgBuf = nullptr;

	DWORD nMsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, m_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	// if length of error string is 0 something went wrong
	if (nMsgLen == 0)
		return "Unidentified error code";

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

Terra::DirectXContext::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	: SBException(line, file)
{
	for (const auto& m : infoMsgs)
	{
		m_Info += m;
		m_Info.push_back('\n');
	}
	// remove final newline if exists
	if (!m_Info.empty())
		m_Info.pop_back();
}

const char* Terra::DirectXContext::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}


Terra::DirectXContext::DirectXContext(HWND hWnd, int width, int height)
	: m_WindowHandle(hWnd), m_Width(width), m_Height(height)
{	
	SB_CORE_INFO("Device Info:");
	infoManager.DisplayDeviceInfo();

	/*SB_CORE_ASSERT(hWnd, "Window handle is null!");
	SB_CORE_INFO("DirectX Info:");
	SB_CORE_INFO("	Vendor: {0}", D3D11_SDK_VERSION);
	SB_CORE_INFO("	Renderer: {0}", "D3D11");
	SB_CORE_INFO("	Version: {0}", D3D11_SHADER_MAJOR_VERSION);*/
}

void Terra::DirectXContext::Init()
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = m_Width;
	sd.BufferDesc.Height = m_Height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // pixel layout
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;		// count = 1 and quality = 0 for no AA
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;		// set to 1 originally
	sd.OutputWindow = m_WindowHandle;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // DXGI_SWAP_EFFECT_DISCARD originally
	sd.Flags = 0;

	//DXGI_SWAP_CHAIN_DESC p = {};
	//pSwap->GetDesc(&p);
	//p.Flags = DXGI_PRESENT_ALLOW_TEARING;

	// D3D11_CREATE_DEVICE_DEBUG creates a device that supports the debug layer.  
	// DxgiInfoManager outputs info from debug onto the exception window
	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	// create device and front/back buffers, and swap chain and rendering context
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		swapCreateFlags, nullptr, 0, D3D11_SDK_VERSION,
		&sd, &pSwapChain, &pDevice, nullptr, &pDeviceContext
	));

	// gain access to texture subresource in swap chain (back buffer)
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer)); // 0 for backbuffer
	// get render view from back buffer's texture
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTarget));

	//create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;  // pixewl with lesser z value is chosen
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));
	// bind depth state to pipeline OM (output merger)
	pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// depth stencil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = m_Width;
	descDepth.Height = m_Height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc = { 1u, 0u };
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// create view of depth stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDepthStencilView));
	// bind depth stencil view to OM
	pDeviceContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDepthStencilView.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)m_Width;
	vp.Height = (float)m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pDeviceContext->RSSetViewports(1u, &vp);

}

void Terra::DirectXContext::SwapBuffers()
{
	
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif 

	if (FAILED(hr = pSwapChain->Present(1u, 0u)))	// 1u to hit default monitor refresh rate (vsync), 2u for half, 0 for no vsync
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		else
			throw GFX_EXCEPT(hr);
	}
}

void Terra::DirectXContext::BeginFrame(float r, float g, float b) noexcept
{
	
}
