#pragma once

#include "ForgeDelta/Core/Base.h"


#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace ForgeDelta {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core Logging Macros
#define FD_CORE_TRACE(...)	ForgeDelta::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FD_CORE_INFO(...)	ForgeDelta::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FD_CORE_WARN(...)	ForgeDelta::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FD_CORE_ERROR(...)	ForgeDelta::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FD_CORE_FATAL(...)	ForgeDelta::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define FD_TRACE(...)	ForgeDelta::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FD_INFO(...)	ForgeDelta::Log::GetClientLogger()->info(__VA_ARGS__)
#define FD_WARN(...)	ForgeDelta::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FD_ERROR(...)	ForgeDelta::Log::GetClientLogger()->error(__VA_ARGS__)
#define FD_FATAL(...)	ForgeDelta::Log::GetClientLogger()->critical(__VA_ARGS__)