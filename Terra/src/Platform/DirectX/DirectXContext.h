#pragma once
#include "Platform/Windows/Win32Config.h"
#include "DXGIInfoManager.h"
#include "Terra/Renderer/GraphicsContext.h"
#include "Terra/Core/Exception.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>


namespace Terra
{
	
	class DirectXContext : public GraphicsContext
	{
		friend class DirectXRendererAPI;
	public:
		// for functions that take in an HRESULT
		class HrException : public SBException
		{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override { return "Graphics Exception"; }
			HRESULT GetErrorCode() const noexcept { return m_Hr; }
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept { return m_Info; }
		private:
			HRESULT m_Hr;
			std::string m_Info;
		};
		class InfoException : public SBException
		{
		public:
			InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override { return "Graphics Info Exception"; }
			std::string GetErrorInfo() const noexcept { return m_Info; }
		private:
			std::string m_Info;
		};
		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			const char* GetType() const noexcept override { return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)"; }
		private:
			std::string m_Reason;
		};

	public:
		DirectXContext(HWND hWnd, int width, int height);
		DirectXContext(const DirectXContext&) = delete;
		DirectXContext operator=(const DirectXContext&) = delete;
		~DirectXContext() = default;

		virtual void Init() override;
		virtual void SwapBuffers() override;
		void BeginFrame(float r, float g, float b) noexcept;
	#ifndef NDEBUG
		DXGIInfoManager infoManager;
	#endif
	private:
		HWND m_WindowHandle;
		int m_Width, m_Height;
		// .getaddressof() to get pointer to pointer, .Get() to get underlying ptr, & to fill ptr (calls release()) 
		//Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		//Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		//Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		//Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		//Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

		static Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		static Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext>  pDeviceContext;
		static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
		static Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	};
}