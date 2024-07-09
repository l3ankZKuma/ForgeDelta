#pragma once

#include "ForgeDelta/Core/Events/Event.h"
#include "ForgeDelta/Core/Window.h"
#include "ForgeDelta/Core/LayerStack.h"
#include"ForgeDelta/Core/TimeStep.h"      


namespace ForgeDelta {

  struct ApplicationData {
    LayerStack m_StackLayer;
    WindowData* m_Window;
    LayerData* m_ImguiLayer;
    bool m_Running = true;
  };

  extern std::atomic<ApplicationData*> g_Application;
  extern float g_lastFrameTime;

  void InitializeApplication(ApplicationData* app);
  void ShutdownApplication(ApplicationData* app);
  void RunApplication(ApplicationData* app);
  void OnApplicationUpdate(ApplicationData* app,TimeStep ts);
  void OnApplicationEvent(ApplicationData* app, Event& e);

  ApplicationData* CreateApplication();
}
