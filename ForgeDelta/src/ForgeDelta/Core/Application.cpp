#include "fdpch.h"
#include "Application.h"

#include "ForgeDelta/Core/Layer.h"
#include "ForgeDelta/Core/Input.h"
#include "ForgeDelta/Core/TimeStep.h"
#include "ForgeDelta/Core/Log.h"
#include "ForgeDelta/ImGui/ImGuiLayer.h"

#include"ForgeDelta/Core/Base.h"
#include"ForgeDelta/Renderer/Renderer.h"
#include"ForgeDelta/Renderer/RendererCommand.h"

#include"ForgeDelta/Debug/Instrumentor.h"


namespace ForgeDelta {

  Application::Application() {
    FD_PROFILE_FUNCTION();  // Profile the Run function

    s_instance = this;  // Correct usage of s_instance

    m_window = new Window();
    InitializeWindow(m_window); // Correctly initialize the window
    SetEventCallback(m_window,BIND_EVENT_FN(Application::OnEvent));  // Correct binding syntax
    m_imGuiLayer = new ImGuiLayer();
    PushOverlay(m_imGuiLayer);



    Renderer::Init();


  }

  Application::~Application() {
    FD_PROFILE_FUNCTION();  // Profile the Run function

    ShutdownWindow(m_window);  // Properly shut down the window
    delete m_window;
  }

  void Application::Run() {

    TimeStep timeStep;




    while (m_running) {  // Correct usage of m_running


      

        float time = static_cast<float>(glfwGetTime());
        timeStep = time - m_lastFrameTime;
        m_lastFrameTime = time;

#if 1
        FD_CORE_INFO("FPS : {0}", 1.0f / timeStep.GetSeconds());

#endif 

        if (!m_minimized) {
          for (Layer* layer : m_layerStack)
            layer->OnUpdate(timeStep);
        }

        m_imGuiLayer->Begin();

        for (Layer* layer : m_layerStack)
          layer->OnImGuiRender();

        m_imGuiLayer->End();

      OnWindowUpdate(m_window);  // Properly update the window
    }
  }

  void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

    dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) -> bool {
      return this->OnWindowResize(event);
      });

    for (auto it = m_layerStack.end(); it != m_layerStack.begin(); ) {
      (*--it)->OnEvent(e);
      if (e.Handled)
        break;
    }
  }

  void Application::PushLayer(Layer* layer) {
    m_layerStack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* overlay) {
    m_layerStack.PushOverlay(overlay);
    overlay->OnAttach();
  }

  bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_running = false;  // Proper usage of m_running
    return true;
  }

  bool Application::OnWindowResize(WindowResizeEvent& e) {
    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
      m_minimized = true;
      return true;
    } 
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
    m_minimized = false;
    return false;
  }
}
