#pragma once

#include <memory>
#include <filesystem>
#include "Terra/Core/Log.h"

#define DIRECTX
// Platform detection
#ifdef _WIN32
	#ifdef _WIN64
		#define SB_PLATFORM_WINDOWS
		#include "Platform/Windows/Win32Config.h"
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
		#define SB_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define SB_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#define TERRA_API __declspec(dllimport)
		#error "Uknown Apple platform!"
	#endif

#elif defined(__ANDROID__)
	#define SB_PLATFORM_ANDROID
	#error "Android is not supported!"
#eilf defined(__linux__)
	#define SB_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#define TERRA_API
#endif


#define SB_EXPAND_MACRO(x) x
#define SB_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

// DLL Support
#ifdef SB_PLATFORM_WINDOWS		// defined in project settings -> preprocessor definitions
	#if SB_DYNAMIC_LINK
		#ifdef SB_BUILD_DLL
			#define  __declspec(dllexport)
		#else
			#define  __declspec(dllimport)
		#endif
	#else
		#define TERRA_API
	#endif
#else
		#error Terra only supports Windows!
#endif

#ifdef SB_DEBUG
	#if defined(SB_PLATFORM_WINDOWS)
		#define SB_DEBUGBREAK() __debugbreak()
	#elif defined(SB_PLATFORM_LINUX)
		#include <signal.h>
		#define SB_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Terra doesn't support debugbreak for this platform yet!"
	#endif
	#define SB_ENABLE_ASSERTS
#else
	#define SB_DEBUGBREAK()
#endif


#ifdef SB_ENABLE_ASSERTS    // TODO: add to premake file for debug
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define SB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { SB##type##ERROR(msg, __VA_ARGS__); SB_DEBUGBREAK(); } }
	#define SB_INTERNAL_ASSERT_WITH_MSG(type, check, ...) SB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define SB_INTERNAL_ASSERT_NO_MSG(type, check) SB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", SB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	
	#define SB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define SB_INTERNAL_ASSERT_GET_MACRO(...) SB_EXPAND_MACRO( SB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, SB_INTERNAL_ASSERT_WITH_MSG, SB_INTERNAL_ASSERT_NO_MSG) )
	
	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define SB_ASSERT(...) SB_EXPAND_MACRO( SB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define SB_CORE_ASSERT(...) SB_EXPAND_MACRO( SB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
	//#define SB_ASSERT(x, ...) {if(!(x)) {SB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	//#define SB_CORE_ASSERT(x, ...) {if(!(x)) {SB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SB_ASSERT(x, ...)
	#define SB_CORE_ASSERT(x, ...)
#endif


// Binds "this" w/ 1 placeholder to function fn)
//#define SB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)  

// decltype(auto) to deduce return type after lambda is declared
#define SB_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Terra {

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	template <typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}