#pragma once
#include "Platform/Windows/Win32Config.h"
#include "DirectXThrowMacros.h"
#include<wrl.h>
#include <dxgidebug.h>
#include <dxgi.h>
#include <string>
#include <vector>

namespace Terra
{
	class DXGIInfoManager
	{
	public:
		DXGIInfoManager();
		~DXGIInfoManager();
		DXGIInfoManager(const DXGIInfoManager&) = delete;
		DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
		void Set() noexcept;
		std::vector<std::string> GetMessages() const;
		void DisplayDeviceInfo();
	private:
		void EnumerateDXGIInfo(IDXGIFactory* pDXGIFactory);
	private:
		unsigned long long next = 0u;
		Microsoft::WRL::ComPtr<struct IDXGIInfoQueue> pDxgiInfoQueue;
	};
}