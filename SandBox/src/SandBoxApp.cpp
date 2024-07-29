#include <ForgeDelta.h>
#include "imgui.h"



using VAO = ForgeDelta::VAO_Data;
using VBO = ForgeDelta::VBO_Data;
using EBO = ForgeDelta::EBO_Data;

/*
class ExampleLayer : public ForgeDelta::Layer {
public:
  ExampleLayer() : Layer("Layers") {}

  void OnAttach() override {
    // Load the shader from the file
    m_ShaderLibrary.Load("BasicShader", "assets/shaders/BasicShader.glsl");
    m_ShaderLibrary.Load("Texture", "assets/shaders/Texture.glsl");
    m_ShaderLibrary.Load("Grid", "assets/shaders/Grid.glsl");
    m_ShaderLibrary.Load("Quad", "assets/shaders/Quad.glsl");

    GLfloat vertices[] = {
      // Position      // Color
      -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom-left vertex (red)
       0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-right vertex (green)
       0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top vertex (blue)
    };

    GLuint indices[] = {
        0, 1, 2
    };

    ForgeDelta::BufferLayout layout = {
        { ForgeDelta::ShaderDataType::Float3, "a_Position" },
        { ForgeDelta::ShaderDataType::Float3, "a_Color" }
    };

    SetupBuffers(m_VAO_Data, m_VBO_Data, m_EBO_Data, vertices, sizeof(vertices), indices, sizeof(indices), layout);

    //----------------------------------------------------------------------------------------------------------------------------

    GLfloat logoVertices[] = {
      0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // Bottom-left vertex (magenta)
     1.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,  // Bottom-right vertex (cyan)
     1.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,  // Top-right vertex (white)
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f   // Top-left vertex (gray)
    };

    GLuint logoIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    ForgeDelta::BufferLayout logoLayout = {
      { ForgeDelta::ShaderDataType::Float3, "a_Position" },
      { ForgeDelta::ShaderDataType::Float3, "a_Color" },
      { ForgeDelta::ShaderDataType::Float2, "a_TexCoord" }
    };

    SetupBuffers(m_LogoVAO_Data, m_LogoVBO_Data, m_LogoEBO_Data, logoVertices, sizeof(logoVertices), logoIndices, sizeof(logoIndices), logoLayout);
    m_LogoTextureID = ForgeDelta::g_TextureSystem.CreateTexture2D("assets/textures/TwitterLogo.png");

    //----------------------------------------------------------------------------------------------------------------------------

    GLfloat checkerVertices[] = {
      0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // Bottom-left vertex (magenta)
     1.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,  // Bottom-right vertex (cyan)
     1.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,  // Top-right vertex (white)
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f   // Top-left vertex (gray)
    };

    GLuint checkerIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    SetupBuffers(m_CheckBoardVAO, m_CheckBoardVBO, m_CheckBoardEBO, checkerVertices, sizeof(checkerVertices), checkerIndices, sizeof(checkerIndices), logoLayout);
    m_CheckBoardTextureID = ForgeDelta::g_TextureSystem.CreateTexture2D("assets/textures/CheckerBoard.png");

    //----------------------------------------------------------------------------------------------------------------------------


  }

  void OnDetach() override {
    // Delete Vertex Buffers
    ForgeDelta::BufferSystem::DeleteVBO(m_VBO_Data);
    ForgeDelta::BufferSystem::DeleteVBO(m_LogoVBO_Data);
    ForgeDelta::BufferSystem::DeleteVBO(m_CheckBoardVBO);

    // Delete Index Buffers
    ForgeDelta::BufferSystem::DeleteEBO(m_EBO_Data);
    ForgeDelta::BufferSystem::DeleteEBO(m_LogoEBO_Data);
    ForgeDelta::BufferSystem::DeleteEBO(m_CheckBoardEBO);

    // Delete Vertex Arrays
    ForgeDelta::VAOSystem::DeleteVertexArray(m_VAO_Data);
    ForgeDelta::VAOSystem::DeleteVertexArray(m_LogoVAO_Data);
    ForgeDelta::VAOSystem::DeleteVertexArray(m_CheckBoardVAO);
  }

  void OnUpdate(ForgeDelta::TimeStep ts) override {
    m_CameraController.OnUpdate(ts);
    ForgeDelta::Renderer::BeginScene(m_CameraController.GetCamera());

    {
      auto& BasicShader = m_ShaderLibrary.Get("BasicShader");
      ForgeDelta::Renderer::Submit(m_VAO_Data, BasicShader, m_model);

      auto& TextureShader2 = m_ShaderLibrary.Get("Texture");
      ForgeDelta::ShaderSystem::BindShader(TextureShader2);
      ForgeDelta::g_TextureSystem.BindTexture(m_CheckBoardTextureID);
      ForgeDelta::ShaderSystem::UploadUniformInt(TextureShader2, "texture1", 0);
      ForgeDelta::Renderer::Submit(m_CheckBoardVAO, TextureShader2, m_model);

      auto& TextureShader = m_ShaderLibrary.Get("Texture");
      ForgeDelta::ShaderSystem::BindShader(TextureShader);
      ForgeDelta::g_TextureSystem.BindTexture(m_LogoTextureID);
      ForgeDelta::ShaderSystem::UploadUniformInt(TextureShader, "texture1", 0);
      ForgeDelta::Renderer::Submit(m_LogoVAO_Data, TextureShader, m_model);
    }

    ForgeDelta::Renderer::EndScene();
  }

  void OnEvent(ForgeDelta::Event& e) override {
    ForgeDelta::EventDispatcher dispatcher(e);
    m_CameraController.OnEvent(e);
  }

  void OnImGuiRender() override {
    ImGui::Begin("Settings");
    ImGui::Text("Hello, World!");
    ImGui::End();
  }

private:
  ForgeDelta::ShaderLibrary m_ShaderLibrary;
  VAO m_VAO_Data;
  VBO m_VBO_Data;
  EBO m_EBO_Data;

  VAO m_LogoVAO_Data;
  VBO m_LogoVBO_Data;
  EBO m_LogoEBO_Data;

  VAO m_CheckBoardVAO;
  VBO m_CheckBoardVBO;
  EBO m_CheckBoardEBO;

  uint32_t m_LogoTextureID;
  uint32_t m_CheckBoardTextureID;

  glm::mat4 m_model{ 1.0f };

  ForgeDelta::OrthographicCamera2DController m_CameraController{ 16.0f / 9.0f, true };

private:

  void SetupBuffers(VAO& vao, VBO& vbo, EBO& ebo, GLfloat* vertices, size_t verticesSize,
    GLuint* indices, size_t indicesSize, const ForgeDelta::BufferLayout& layout) {
    ForgeDelta::VAOSystem::CreateVertexArray(vao);

    vbo.Data = vertices;
    vbo.Size = verticesSize;
    vbo.Layout = layout;
    ForgeDelta::BufferSystem::CreateVBO(vbo);

    ebo.Data = indices;
    ebo.Size = indicesSize;
    ebo.Count = indicesSize / sizeof(GLuint);
    ForgeDelta::BufferSystem::CreateEBO(ebo);

    ForgeDelta::VAOSystem::AddVertexBuffer(vao, &vbo);
    ForgeDelta::VAOSystem::SetIndexBuffer(vao, &ebo);
  }
};
*/


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

    ForgeDelta::RenderCommand::SetClearColor({ 0.f, 0.f, 0.f, 1 });
    ForgeDelta::RenderCommand::Clear();

    /*
    * 
    ForgeDelta::Renderer2D::BeginScene(m_CameraController.GetCamera());

    // Render
    {
      FD_PROFILE_SCOPE("SandBox2D::OnUpdate - Render");
      // Example usage of Renderer2D to draw quads
      ForgeDelta::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red Quad
      ForgeDelta::Renderer2D::DrawQuad(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green Quad
      ForgeDelta::Renderer2D::DrawQuad(glm::vec2(0.0f, 3.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue Quad

      // Rotated Quad (spinning)
      ForgeDelta::Renderer2D::DrawRotatedQuad(glm::vec2(2.0f, 2.0f), glm::vec2(3.0f, 3.0f), m_Rotation, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow Rotated Quad

      // Texture
      ForgeDelta::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -1.f), glm::vec2(10.f, 10.f), m_CheckerBoardTextureID, 10.f);

      // Render a grid of quads
      for (float x = -5.0f; x < 5.0f; x += 0.5f) {
        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
          glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 1.0f };
          ForgeDelta::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
        }
      }

      

    }

    ForgeDelta::Renderer2D::EndScene();
     */





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