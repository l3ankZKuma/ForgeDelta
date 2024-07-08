#pragma once

#include "ForgeDelta/Core/Events/Event.h"
#include "ForgeDelta/Core/Window.h"
#include"ForgeDelta/Core/LayerStack.h"

namespace ForgeDelta {

  struct Application {
    LayerStack m_StackLayer;
    WindowData* m_Window;
    bool m_Running = true;
  };

  void InitializeApplication(Application* app);
  void ShutdownApplication(Application* app);
  void RunApplication(Application* app);
  void OnApplicationUpdate(Application* app);
  void OnApplicationEvent(Application* app, Event& e);

  Application* CreateApplication();
}
