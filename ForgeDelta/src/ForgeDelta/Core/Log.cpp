#include"fdpch.h"
#include "Log.h"

namespace ForgeDelta {

  static LogSystemData g_LogSystemData;

  void
  InitLogSystem() {
    spdlog::set_pattern("%^[%T] %n: %v%$");

    g_LogSystemData.coreLogger.logger = spdlog::stdout_color_mt("FORGEDELTA");
    g_LogSystemData.coreLogger.logger->set_level(spdlog::level::trace);

    g_LogSystemData.clientLogger.logger = spdlog::stdout_color_mt("APP");
    g_LogSystemData.clientLogger.logger->set_level(spdlog::level::trace);
  }

  void 
  LogCoreError(const char * message) {
    g_LogSystemData.coreLogger.logger->error(message);
  }

  void 
  LogCoreWarn(const char * message) {
    g_LogSystemData.coreLogger.logger->warn(message);
  }

  void 
  LogCoreInfo(const char * message) {
    g_LogSystemData.coreLogger.logger->info(message);
  }

  void 
  LogCoreTrace(const char * message) {
    g_LogSystemData.coreLogger.logger->trace(message);
  }

  void 
  LogCoreFatal(const char * message) {
    g_LogSystemData.coreLogger.logger->critical(message);
  }

  void 
  LogClientError(const char * message) {
    g_LogSystemData.clientLogger.logger->error(message);
  }

  void LogClientWarn(const char * message) {
    g_LogSystemData.clientLogger.logger->warn(message);
  }

  void LogClientInfo(const char * message) {
    g_LogSystemData.clientLogger.logger->info(message);
  }

  void LogClientTrace(const char * message) {
    g_LogSystemData.clientLogger.logger->trace(message);
  }

  void LogClientFatal(const char * message) {
    g_LogSystemData.clientLogger.logger->critical(message);
  }
}
