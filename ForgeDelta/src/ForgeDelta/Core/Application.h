#pragma once

#include "ForgeDelta/Core/Window.h"
#include "ForgeDelta/Core/LayerStack.h"
#include "ForgeDelta/Core/Events/Event.h"
#include "ForgeDelta/Core/Events/ApplicationEvent.h"
#include "ForgeDelta/Core/Timer.h" // Include the Timer header

namespace ForgeDelta {
  class TimeStep;
  class Event;
  class ImGuiLayer;

}

namespace ForgeDelta {

  class Application {

  public:
    Application();
    ~Application();

    void Run();
    void OnEvent(Event& e);
    void OnUpdate(TimeStep ts);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    inline static Application& Get() { return *s_instance; }
    inline Window& GetWindow() { return *m_window; }
    PerformanceProfiler& GetPerformanceProfiler() { return m_PerformanceProfiler; } // Add this line

  private:
    Window* m_window;
    bool m_minimized{ false };
    bool m_running{ true };

    LayerStack m_layerStack;
    ImGuiLayer* m_imGuiLayer;

    float m_lastFrameTime{ 0.0f };

    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    constinit inline static constinit Application* s_instance{ nullptr };
    PerformanceProfiler m_PerformanceProfiler; // Add this line
  };

  Application* CreateApplication();
}
