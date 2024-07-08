#pragma once

#include "ForgeDelta/Core/Events/Event.h"
#include "ForgeDelta/Core/Window.h"

namespace ForgeDelta {

  struct Application {
    WindowData* m_Window;
    bool m_Running = true;
  };

  void InitializeApplication(Application* app);
  void ShutdownApplication(Application* app);
  void RunApplication(Application* app);
  void OnUpdate(Application* app);
  void OnEvent(Application* app, Event& e);

  Application* CreateApplication();
}
