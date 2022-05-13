#include "Terrapch.h"
#include "DXGIInfoManager.h"
#include "Platform/Windows/Win32Window.h"
#include "DirectXContext.h"

#pragma comment(lib, "dxguid.lib")

Terra::DXGIInfoManager::DXGIInfoManager()
{
	// define function sig for DXGIGetDebugInterface
	typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);  	
	// load all that contains function DXGIGetDebugInterace
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
		throw WND_LAST_EXCEPT();

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
		throw WND_LAST_EXCEPT();
	 
	HRESULT hr;
	// call DXGIGetDebugInterface to get interface
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue));
}

Terra::DXGIInfoManager::~DXGIInfoManager()
{
	if (pDxgiInfoQueue != nullptr)
		pDxgiInfoQueue->Release();
}

void Terra::DXGIInfoManager::Set() noexcept
{
	// set the index (next) so that the next all to GetMessages() will only get errors generated after this call
	next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

void Terra::DXGIInfoManager::DisplayDeviceInfo()
{

	// https://github.com/walbourn/directx-sdk-samples/blob/main/VideoMemory/VideoMemory.cpp
	HINSTANCE hDXGI = LoadLibrary(L"dxgi.dll");
	if (hDXGI)
	{
		typedef HRESULT(WINAPI* LPCREATEDXGIFACTORY)(REFIID, void**);

		LPCREATEDXGIFACTORY pCreateDXGIFactory = nullptr;
		IDXGIFactory* pDXGIFactory = nullptr;

		pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory1");

		if (!pCreateDXGIFactory)
		{
			pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory");

			if (!pCreateDXGIFactory)
			{
				FreeLibrary(hDXGI);
				wprintf(L"ERROR: dxgi.dll missing entry-point\n");
				return;
			}
		}

		HRESULT hr = pCreateDXGIFactory(__uuidof(IDXGIFactory), (LPVOID*)&pDXGIFactory);

		if (SUCCEEDED(hr))
		{
			SB_ASSERT(pDXGIFactory != NULL);
			EnumerateDXGIInfo(pDXGIFactory);

			pDXGIFactory->Release();
			pDXGIFactory = nullptr;

			return;
		}

		FreeLibrary(hDXGI);

	}
}

void Terra::DXGIInfoManager::EnumerateDXGIInfo(IDXGIFactory* pDXGIFactory)
{
	for (UINT index = 0; ; ++index)
	{
		IDXGIAdapter* pAdapter = nullptr;
		HRESULT hr = pDXGIFactory->EnumAdapters(index, &pAdapter);
		if (FAILED(hr)) // DXGIERR_NOT_FOUND is expected when the end of the list is hit
			break;

		DXGI_ADAPTER_DESC desc;
		memset(&desc, 0, sizeof(DXGI_ADAPTER_DESC));
		if (SUCCEEDED(pAdapter->GetDesc(&desc)))
		{
			wprintf(L"DXGI Adapter Description: %s\n", desc.Description);
			for (UINT iOutput = 0; ; ++iOutput)
			{
				IDXGIOutput* pOutput = nullptr;
				hr = pAdapter->EnumOutputs(iOutput, &pOutput);
				if (FAILED(hr)) // DXGIERR_NOT_FOUND is expected when the end of the list is hit
					break;

				DXGI_OUTPUT_DESC outputDesc;
				memset(&outputDesc, 0, sizeof(DXGI_OUTPUT_DESC));
				if (SUCCEEDED(pOutput->GetDesc(&outputDesc)))
				{
					wprintf(L"\thMonitor: 0x%0.8Ix\n", (DWORD_PTR)outputDesc.Monitor);
					wprintf(L"\thMonitor Device Name: %s\n", outputDesc.DeviceName);
				}

				pOutput->Release();
				pOutput = nullptr;
			}

			wprintf(
				L"\tDedicatedVideoMemory: %zu MB\n\tDedicatedSystemMemory: %zu MB\n\tSharedSystemMemory: %zu MB\n\n",
				desc.DedicatedVideoMemory / 1024 / 1024,
				desc.DedicatedSystemMemory / 1024 / 1024,
				desc.SharedSystemMemory / 1024 / 1024);

		}

		pAdapter->Release();
		pAdapter = nullptr;
	}
}

std::vector<std::string> Terra::DXGIInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;
		// get the size of message i in bytes
		GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push its description into the vector
		GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}
