#pragma once
#define FMT_USE_NONTYPE_TEMPLATE_PARAMETERS 0
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Terra {
	class Log
	{
	public:
		static void Init();

		// Return by reference so ref count doesn't increment
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// ... for multiple args		__VA_ARGS__ forwards macro args to the function
#define CORE_TRACE(...) ::Terra::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::Terra::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) ::Terra::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::Terra::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) ::Terra::Log::GetCoreLogger()->critical(__VA_ARGS__)

/* Core Log Macros */
#define TERRA_TRACE(...) ::Terra::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TERRA_INFO(...) ::Terra::Log::GetClientLogger()->info(__VA_ARGS__)
#define TERRA_WARN(...) ::Terra::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TERRA_ERROR(...) ::Terra::Log::GetClientLogger()->error(__VA_ARGS__)
#define TERRA_FATAL(...) ::Terra::Log::GetClientLogger()->critical(__VA_ARGS__)


// if dist build
//#define SB_CORE_INFO   // set macro to nothing