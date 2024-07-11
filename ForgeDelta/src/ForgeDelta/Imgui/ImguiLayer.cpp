#include "fdpch.h"
#include "ImGuiLayer.h"

// imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"



#include"ForgeDelta/Core/Events/Event.h"
#include"ForgeDelta/Core/Window.h"
#include"ForgeDelta/Core/Application.h"

#include"ForgeDelta/Core/TimeStep.h"

namespace ForgeDelta {

  static Window* m_window = nullptr;

  ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") {
  }

  ImGuiLayer::~ImGuiLayer() {}

  void ImGuiLayer::OnAttach() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    auto &app = Application::Get();
    m_window = &app.GetWindow();
    ImGui_ImplGlfw_InitForOpenGL(m_window->GLFWWindow, true);
    ImGui_ImplOpenGL3_Init("#version 410");
  }

  void ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiLayer::OnEvent(Event& e) {
    if (m_BlockEvents) {
      ImGuiIO& io = ImGui::GetIO();
      e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
      e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
  }

  void ImGuiLayer::OnUpdate(TimeStep ts) {}

  void ImGuiLayer::OnImGuiRender() {

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

  }

  void ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiLayer::End() {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)m_window->Width,(float)m_window->Height);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
  }

}
