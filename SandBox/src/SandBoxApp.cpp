#include <ForgeDelta.h>
#include "imgui.h"

using VAO = ForgeDelta::VAO_Data;
using VBO = ForgeDelta::VBO_Data;
using EBO = ForgeDelta::EBO_Data;

class SandBox2D : public ForgeDelta::Layer {
public:
  SandBox2D()
    : Layer("SandBox2D"),
    m_Rotation(0.0f)
  {}
  virtual ~SandBox2D() = default;

  void OnAttach() override {
    FD_PROFILE_FUNCTION(); // Profile the OnAttach function

    // Initialize the ImGui context here
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ForgeDelta::FramebufferSpecification fbSpec;

    fbSpec.Attachments = { ForgeDelta::FramebufferTextureFormat::RGBA8, ForgeDelta::FramebufferTextureFormat::RED_INTEGER, ForgeDelta::FramebufferTextureFormat::DEPTH24STENCIL8 };
    fbSpec.Width = 1280;
    fbSpec.Height = 720;

    m_FrameBuffer.create(fbSpec);

    ForgeDelta::Renderer2D::Init();
  }

  void OnDetach() override {
    FD_PROFILE_FUNCTION(); // Profile the OnDetach function
    ForgeDelta::Renderer2D::Shutdown();

    // Cleanup ImGui context
    ImGui::DestroyContext();
  }

  void OnUpdate(ForgeDelta::TimeStep ts) override {
    FD_PROFILE_FUNCTION(); // Profile the OnUpdate function

    m_FrameBuffer.Bind();

    ForgeDelta::Renderer2D::ResetStats();

    // Update the camera controller
    m_CameraController.OnUpdate(ts);

    // Update rotation angle
    m_Rotation += ts * 50.0f;

    // Rendering logic
    ForgeDelta::Renderer2D::BeginScene(m_CameraController.GetCamera());
    {



    }
    ForgeDelta::Renderer2D::EndScene();
    m_FrameBuffer.Unbind();
  }

  void OnEvent(ForgeDelta::Event& e) override {
    FD_PROFILE_FUNCTION(); // Profile the OnEvent function
    m_CameraController.OnEvent(e);
  }

  void OnImGuiRender() override {
    FD_PROFILE_FUNCTION(); // Profile the OnImGuiRender function

    static bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
      const ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (!opt_padding)
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace Demo", &p_open, window_flags);

    if (!opt_padding)
      ImGui::PopStyleVar();

    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {

        if (ImGui::MenuItem("Exit")) ForgeDelta::Application::Get().Close();

        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Grid Color", glm::value_ptr(uColor));

    ImGui::Text("Renderer2D Stats:");
    auto stats = ForgeDelta::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    uint32_t textureID = m_FrameBuffer.GetColorAttachmentRendererID();

    //ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });



    ImGui::End();
    ImGui::End();
  }

private:
  ForgeDelta::OrthographicCamera2DController m_CameraController{ 16.0f / 9.0f, true };
  glm::vec4 uColor{ 0.2f, 0.3f, 0.8f, 1.0f };


  ForgeDelta::Framebuffer m_FrameBuffer;

  float m_Rotation;
};

class SandBox : public ForgeDelta::Application {
public:
  SandBox() {
    PushLayer(new SandBox2D());
  }

  ~SandBox() {}
};

ForgeDelta::Application* ForgeDelta::CreateApplication() {
  return new SandBox();
}
