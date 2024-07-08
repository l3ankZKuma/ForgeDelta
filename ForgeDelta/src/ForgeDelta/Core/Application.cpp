#include "fdpch.h"
#include "Application.h"
#include "ForgeDelta/Core/Base.h"

namespace ForgeDelta {

  void InitializeApplication(Application* app) {
    ForgeDelta::InitLogSystem();

    app->m_Window = new WindowData();
    InitializeWindow(app->m_Window);
    SetEventCallback(app->m_Window, [&app](Event& e) {
      OnApplicationEvent(app, e);
     });

    app->m_StackLayer = LayerStack();

  }

  void ShutdownApplication(Application* app) {
    ShutdownWindow(app->m_Window);
    delete app->m_Window;
  }

  void RunApplication(Application* app) {

    while (app->m_Running) {
      glClearColor(.5f, 0.1f, 0.f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      OnApplicationUpdate(app);
    }
  }

  void OnApplicationUpdate(Application* app) {
    OnWindowUpdate(app->m_Window);
  }

  void OnApplicationEvent(Application* app, Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>([app](WindowCloseEvent& e) {
      app->m_Running = false;
      return true;
      });
  }

}
