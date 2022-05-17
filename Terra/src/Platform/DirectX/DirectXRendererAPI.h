#pragma once
#include "Terra/Renderer/RendererAPI.h"
#include "Terra/Core/Application.h"
#include "Platform/Windows/Win32Window.h"
#include "DirectXContext.h"
#include "DirectXThrowMacros.h"
namespace Terra
{

	class DirectXRendererAPI : public RendererAPI
	{
		
	public:
		virtual void Init() override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;	
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void SetLineWidth(float width) override;
					
		static Ref<DirectXContext>& GetDXGraphicsContext() { return m_DXContext; }	
		static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return DirectXContext::pDevice; }
		static Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() { return DirectXContext::pSwapChain; }
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext>  GetDeviceContext() { return DirectXContext::pDeviceContext; }
		static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return DirectXContext::pRenderTarget; }
		static Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return DirectXContext::pDepthStencilView; }

	private:
#ifndef NDEBUG
		DXGIInfoManager& GetInfoManager(DirectXContext& gfx) { return gfx.infoManager; }
#endif
		static Ref<DirectXContext> m_DXContext;
	};
}