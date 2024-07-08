#pragma once

namespace ForgeDelta {

  struct Application {

  };

  void InitializeApplication(Application* app);
  void ShutdownApplication(Application* app);
  void RunApplication(Application* app);
  void OnUpdate(Application* app);
  // void OnEvent(Application& app, Event& e);

  Application* CreateApplication();
}
