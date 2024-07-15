#pragma once

#ifdef FD_PLATFORM_WINDOWS

#include "ForgeDelta/Core/Application.h"
#include "ForgeDelta/Core/Log.h"
#include "ForgeDelta/Debug/Instrumentor.h"

extern ForgeDelta::Application* ForgeDelta::CreateApplication();

int main(int argc, char** argv) {
  ForgeDelta::Log::Init();

  FD_PROFILE_BEGIN_SESSION("Startup", "ForgeDelta-Startup.json");
  auto app = ForgeDelta::CreateApplication();
  FD_PROFILE_END_SESSION();

  FD_PROFILE_BEGIN_SESSION("Runtime", "ForgeDelta-Runtime.json");
  app->Run();
  FD_PROFILE_END_SESSION();

  FD_PROFILE_BEGIN_SESSION("Shutdown", "ForgeDelta-Shutdown.json");
  delete app;
  FD_PROFILE_END_SESSION();

  return 0;
}

#endif // FD_PLATFORM_WINDOWS
