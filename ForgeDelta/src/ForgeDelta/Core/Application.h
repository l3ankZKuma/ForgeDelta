#pragma once

#include "ForgeDelta/Core/Events/Event.h"
#include "ForgeDelta/Core/Window.h"
#include"ForgeDelta/Core/LayerStack.h"

namespace ForgeDelta {

  struct ApplicationData {
    LayerStack m_StackLayer;
    WindowData* m_Window;
    LayerData *imguiLayer;
    bool m_Running = true;
  };

  extern std::atomic<ApplicationData*> g_Application;  // Use atomic for thread safety

  void InitializeApplication(ApplicationData* app);
  void ShutdownApplication(ApplicationData* app);
  void RunApplication(ApplicationData* app);
  void OnApplicationUpdate(ApplicationData* app);
  void OnApplicationEvent(ApplicationData* app, Event& e);

  ApplicationData* CreateApplication();
}
