#include <ForgeDelta.h>
#include "imgui.h"



using VAO = ForgeDelta::VAO_Data;
using VBO = ForgeDelta::VBO_Data;
using EBO = ForgeDelta::EBO_Data;


class SandBox2D : public ForgeDelta::Layer {
public:
  SandBox2D() : Layer("SandBox2D"), m_Rotation(0.0f) {}
  virtual ~SandBox2D() = default;

  void OnAttach() override {
    FD_PROFILE_FUNCTION(); // Profile the OnAttach function

    m_SpriteSheet = ForgeDelta::g_TextureSystem.CreateTexture2D("assets/Game/RPGpack_sheet_2X.png");


    m_GrassTile = ForgeDelta::g_TextureSystem.CreateSubTextureFromCoords(m_SpriteSheet,{0,1664-381},{381, 381});


    ForgeDelta::Renderer2D::Init();
  }

  void OnDetach() override {
    FD_PROFILE_FUNCTION(); // Profile the OnDetach function
    ForgeDelta::Renderer2D::Shutdown();
  }

  void OnUpdate(ForgeDelta::TimeStep ts) override {
    FD_PROFILE_FUNCTION(); // Profile the OnUpdate function

    ForgeDelta::Renderer2D::ResetStats();

    // Update
    {
      FD_PROFILE_SCOPE("SandBox2D::OnUpdate - CameraController Update");
      m_CameraController.OnUpdate(ts);
    }

    // Update rotation angle
    m_Rotation += ts * 50.0f; // Adjust the speed as needed

    ForgeDelta::Renderer::Clear(0.2f, 0.3f, 0.8f, 1);
     


    ForgeDelta::Renderer2D::BeginScene(m_CameraController.GetCamera());
    {


      //Draw the grass tile
      ForgeDelta::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_GrassTile,true);

    }
    ForgeDelta::Renderer2D::EndScene();
  }



  void OnEvent(ForgeDelta::Event& e) override {
    FD_PROFILE_FUNCTION(); // Profile the OnEvent function
    ForgeDelta::EventDispatcher dispatcher(e);
    m_CameraController.OnEvent(e);
  }

  void OnImGuiRender() override {
    FD_PROFILE_FUNCTION(); // Profile the OnImGuiRender function
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Grid Color", glm::value_ptr(uColor));


    ImGui::Text("Renderer2D Stats:");
    auto stats = ForgeDelta::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());




    ImGui::End();
  }

private:
  ForgeDelta::OrthographicCamera2DController m_CameraController{ 16.0f / 9.0f, true };
  glm::vec4 uColor{ 0.2f, 0.3f, 0.8f, 1.0f };


  uint32_t m_SpriteSheet;
  uint32_t m_GrassTile;
  uint32_t m_WaterTile;
  uint32_t m_TreeSprite;
  uint32_t m_HouseWallBeige;
  uint32_t m_HouseRoofBrown;
  uint32_t m_WindowRoundArch;
  
  
  
  
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