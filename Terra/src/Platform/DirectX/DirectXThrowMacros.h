#pragma once
// window exception helper macro
#define WND_EXCEPT(hr) Win32Window::HrException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT() Win32Window::HrException(__LINE__, __FILE__, GetLastError())
#define WND_NOGFX_EXCEPT() Win32Window::NoGfxException(__LINE__, __FILE__)


// graphics context exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) DirectXContext::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw DirectXContext::HrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) DirectXContext::HrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED(hr = (hrcall)) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DirectXContext::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); if(!infoManager.GetMessages().empty()) throw DirectXContext::InfoException(__LINE__, __FILE__, infoManager.GetMessages());
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

// macro for importing infomanager into local scope
// this.GetInfoManager() must exist
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; DXGIInfoManager& infoManager = GetInfoManager(gfx)
#endif