#include "fdpch.h"
#include "Layer.h"

// imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


// Application
#include "ForgeDelta/Core/Application.h"

namespace ForgeDelta {

  void OnLayerAttach(LayerData* layerData) {
    switch (layerData->m_Type) {
    case LayerType::BaseLayer:
      break;
    case LayerType::ImGuiLayer:
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
      io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
      io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

      ImGui::StyleColorsDark();

      ImGuiStyle& style = ImGui::GetStyle();
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
      }

      ApplicationData* app = g_Application.load(std::memory_order_relaxed);
      GLFWwindow* window = app->m_Window->GLFWWindow;

      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init("#version 410");
      break;
    }
  }

  void OnLayerDetach(LayerData* layerData) {
    switch (layerData->m_Type) {
    case LayerType::BaseLayer:
      break;
    case LayerType::ImGuiLayer:
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
      break;
    }
  }


  void OnLayerUpdate(LayerData* layerData, TimeStep ts) {
    switch (layerData->m_Type) {
    case LayerType::BaseLayer:
      break;
    case LayerType::ImGuiLayer:
      break;
    }
  }

  void OnLayerEvent(LayerData* layerData, Event& e) {
    switch (layerData->m_Type) {
    case LayerType::BaseLayer:

      break;
    case LayerType::ImGuiLayer:

      break;
    }
  }


  void OnImGuiRender(LayerData* layerData) {
    switch (layerData->m_Type) {
    case LayerType::BaseLayer:
      break;
    case LayerType::ImGuiLayer:
      bool show = true;
      ImGui::ShowDemoWindow(&show);
      break;
    }
  }
void BeginImGuiLayer() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EndImGuiLayer() {
    ImGuiIO& io = ImGui::GetIO();
    ApplicationData* app = g_Application.load(std::memory_order_relaxed);
    io.DisplaySize = ImVec2{ (float)app->m_Window->Width, (float)app->m_Window->Height };

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
