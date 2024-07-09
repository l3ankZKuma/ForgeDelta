#include "fdpch.h"
#include "Application.h"
#include "ForgeDelta/Core/Base.h"

namespace ForgeDelta {

  std::atomic<ApplicationData*> g_Application(nullptr);
  float g_lastFrameTime = 0.0f;

  void InitializeApplication(ApplicationData* app) {
    ForgeDelta::InitLogSystem();

    g_Application.store(app, std::memory_order_relaxed);

    app->m_Window = new WindowData();
    InitializeWindow(app->m_Window);
    SetEventCallback(app->m_Window, [&app](Event& e) {
      OnApplicationEvent(app, e);
      });

    app->m_StackLayer = LayerStack();

    app->m_ImguiLayer = new LayerData();
    app->m_ImguiLayer->m_Type = LayerType::ImGuiLayer;
    PushOverlay(app->m_StackLayer, app->m_ImguiLayer);
  }

  void ShutdownApplication(ApplicationData* app) {
    ShutdownLayerStack(app->m_StackLayer);
    ShutdownWindow(app->m_Window);
    delete app->m_Window;
  }

  void RunApplication(ApplicationData* app) {
    TimeStep timeStep;

    while (app->m_Running) {
      // Calculate the time step
      float time = (float)glfwGetTime();
      timeStep = time - g_lastFrameTime;
      g_lastFrameTime = time;


      // Update the application
      OnApplicationUpdate(app, timeStep);


    }
  }


  void OnApplicationUpdate(ApplicationData* app, TimeStep ts) {

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    BeginImGuiLayer();

  {
    OnImGuiRender(app->m_ImguiLayer);
  }

    EndImGuiLayer();



    OnWindowUpdate(app->m_Window);

  }

  void OnApplicationEvent(ApplicationData* app, Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>([app](WindowCloseEvent& e) {
      app->m_Running = false;
      return true;
      });
  }
}
