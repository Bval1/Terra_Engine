#pragma once

#include <memory>
#include <filesystem>
#include "Terra/Core/Log.h"

#define DIRECTX
// Platform detection
#ifdef _WIN32
	#ifdef _WIN64
		#define TERRA_PLATFORM_WINDOWS
		#include "Platform/Windows/Win32Config.h"
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
		#define TERRA_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define TERRA_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#define TERRA_API __declspec(dllimport)
		#error "Uknown Apple platform!"
	#endif

#elif defined(__ANDROID__)
	#define TERRA_PLATFORM_ANDROID
	#error "Android is not supported!"
#eilf defined(__linux__)
	#define TERRA_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#define TERRA_API
#endif


#define TERRA_EXPAND_MACRO(x) x
#define TERRA_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

// DLL Support
#ifdef TERRA_PLATFORM_WINDOWS		// defined in project settings -> preprocessor definitions
	#if TERRA_DYNAMIC_LINK
		#ifdef TERRA_BUILD_DLL
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

#ifdef TERRA_DEBUG
	#if defined(TERRA_PLATFORM_WINDOWS)
		#define TERRA_DEBUGBREAK() __debugbreak()
	#elif defined(TERRA_PLATFORM_LINUX)
		#include <signal.h>
		#define TERRA_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Terra doesn't support debugbreak for this platform yet!"
	#endif
	#define TERRA_ENABLE_ASSERTS
#else
	#define TERRA_DEBUGBREAK()
#endif


#ifdef TERRA_ENABLE_ASSERTS    // TODO: add to premake file for debug
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define TERRA_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { TERRA##type##ERROR(msg, __VA_ARGS__); TERRA_DEBUGBREAK(); } }
	#define TERRA_INTERNAL_ASSERT_WITH_MSG(type, check, ...) TERRA_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define TERRA_INTERNAL_ASSERT_NO_MSG(type, check) TERRA_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", TERRA_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	
	#define TERRA_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define TERRA_INTERNAL_ASSERT_GET_MACRO(...) TERRA_EXPAND_MACRO( TERRA_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, TERRA_INTERNAL_ASSERT_WITH_MSG, TERRA_INTERNAL_ASSERT_NO_MSG) )
	
	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define TERRA_ASSERT(...) TERRA_EXPAND_MACRO( TERRA_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define TERRA_CORE_ASSERT(...) TERRA_EXPAND_MACRO( TERRA_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
	//#define TERRA_ASSERT(x, ...) {if(!(x)) {TERRA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	//#define TERRA_CORE_ASSERT(x, ...) {if(!(x)) {TERRA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define TERRA_ASSERT(x, ...)
	#define TERRA_CORE_ASSERT(x, ...)
#endif


// Binds "this" w/ 1 placeholder to function fn)
//#define TERRA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)  

// decltype(auto) to deduce return type after lambda is declared
#define TERRA_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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