#pragma once


#ifdef FD_PLATFORM_WINDOWS

extern ForgeDelta::Application* ForgeDelta::CreateApplication();

int main(int argc,char **argv) {

  auto app = ForgeDelta::CreateApplication();
  ForgeDelta::InitializeApplication(app);
  ForgeDelta::RunApplication(app);
  ForgeDelta::ShutdownApplication(app);




  return 0;
}


#endif // FORGEDELTA_ENTRYPOINT_H