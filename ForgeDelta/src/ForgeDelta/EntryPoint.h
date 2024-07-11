#pragma once


#ifdef FD_PLATFORM_WINDOWS

extern ForgeDelta::Application* ForgeDelta::CreateApplication();
int main(int argc,char **argv) {

  ForgeDelta::Log::Init();
  auto app = ForgeDelta::CreateApplication();
  app->Run();
  delete app;
  return 0;
}


#endif // FORGEDELTA_ENTRYPOINT_H