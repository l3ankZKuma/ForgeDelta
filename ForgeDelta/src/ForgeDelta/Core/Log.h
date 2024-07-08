#pragma once

//spdlog
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace ForgeDelta {

  struct Logger {
    std::shared_ptr<spdlog::logger> logger;
  };

  struct LogSystemData {
    Logger coreLogger;
    Logger clientLogger;
  };

  extern LogSystemData g_LogSystemData;

  void InitLogSystem();
  void LogCoreError(const char * message); 
  void LogCoreWarn(const char * message);
  void LogCoreInfo(const char * message);
  void LogCoreTrace(const char * message);
  void LogCoreFatal(const char * message);
  void LogClientError(const char * message);
  void LogClientWarn(const char * message);
  void LogClientInfo(const char * message);
  void LogClientTrace(const char * message);
  void LogClientFatal(const char * message);

}

// Core log macros
#define FD_CORE_ERROR(message)  ::ForgeDelta::LogCoreError(message)
#define FD_CORE_WARN(message)   ::ForgeDelta::LogCoreWarn(message)
#define FD_CORE_INFO(message)   ::ForgeDelta::LogCoreInfo(message)
#define FD_CORE_TRACE(message)  ::ForgeDelta::LogCoreTrace(message)
#define FD_CORE_FATAL(message)  ::ForgeDelta::LogCoreFatal(message)

// Client log macros
#define FD_ERROR(message)  ::ForgeDelta::LogClientError(message)
#define FD_WARN(message)   ::ForgeDelta::LogClientWarn(message)
#define FD_INFO(message)   ::ForgeDelta::LogClientInfo(message)
#define FD_TRACE(message)  ::ForgeDelta::LogClientTrace(message)
#define FD_FATAL(message)  ::ForgeDelta::LogClientFatal(message)
