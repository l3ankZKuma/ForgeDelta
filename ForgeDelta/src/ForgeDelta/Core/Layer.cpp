#include"fdpch.h"
#include "Layer.h"

//imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

//Application
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

      // Access the existing ApplicationData instance
      ApplicationData* app = g_Application.load(std::memory_order_relaxed);
      GLFWwindow* window = app->m_Window->GLFWWindow;  // Retrieve the GLFWwindow directly


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
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      break;
    }
  }


  void OnLayerEvent(LayerData* layerData, Event& e) {
    switch (layerData->m_Type) {
    case LayerType::BaseLayer:
      break;
    case LayerType::ImGuiLayer:
      if (layerData->m_BlockEvents) {
        ImGuiIO& io = ImGui::GetIO();
        e.Handled |= e.IsInCategory(EventCategory::EventCategoryMouse) & io.WantCaptureMouse;
        e.Handled |= e.IsInCategory(EventCategory::EventCategoryKeyboard) & io.WantCaptureKeyboard;
      }
      break;
    }
  }

  void OnImGuiRender(LayerData* layerData) {
    switch (layerData->m_Type) {
    case LayerType::BaseLayer:
      break;
    case LayerType::ImGuiLayer:
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      break;
    }
  }





}