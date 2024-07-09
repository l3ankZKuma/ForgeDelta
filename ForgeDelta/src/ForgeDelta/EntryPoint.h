#pragma once


#ifdef FD_PLATFORM_WINDOWS

extern ForgeDelta::ApplicationData* ForgeDelta::CreateApplication();
extern void SandBox();
int main(int argc,char **argv) {

  auto app = ForgeDelta::CreateApplication();

  InitializeApplication(app);

  SandBox();
  RunApplication(app);

  ShutdownApplication(app);
  delete app;

  return 0;
}


#endif // FORGEDELTA_ENTRYPOINT_H