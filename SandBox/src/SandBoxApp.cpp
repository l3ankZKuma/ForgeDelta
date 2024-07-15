#include <ForgeDelta.h>
#include "imgui.h"



using VAO = ForgeDelta::VertexArrayData;
using VBO = ForgeDelta::VertexBufferData;
using EBO = ForgeDelta::IndexBufferData;

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

    SetupBuffers(m_VertexArrayData, m_VertexBufferData, m_IndexBufferData, vertices, sizeof(vertices), indices, sizeof(indices), layout);

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

    SetupBuffers(m_LogoVertexArrayData, m_LogoVertexBufferData, m_LogoIndexBufferData, logoVertices, sizeof(logoVertices), logoIndices, sizeof(logoIndices), logoLayout);
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
    ForgeDelta::OpenGLBufferService::DeleteVertexBuffer(m_VertexBufferData);
    ForgeDelta::OpenGLBufferService::DeleteVertexBuffer(m_LogoVertexBufferData);
    ForgeDelta::OpenGLBufferService::DeleteVertexBuffer(m_CheckBoardVBO);

    // Delete Index Buffers
    ForgeDelta::OpenGLBufferService::DeleteIndexBuffer(m_IndexBufferData);
    ForgeDelta::OpenGLBufferService::DeleteIndexBuffer(m_LogoIndexBufferData);
    ForgeDelta::OpenGLBufferService::DeleteIndexBuffer(m_CheckBoardEBO);

    // Delete Vertex Arrays
    ForgeDelta::OpenGLVertexArrayService::DeleteVertexArray(m_VertexArrayData);
    ForgeDelta::OpenGLVertexArrayService::DeleteVertexArray(m_LogoVertexArrayData);
    ForgeDelta::OpenGLVertexArrayService::DeleteVertexArray(m_CheckBoardVAO);
  }

  void OnUpdate(ForgeDelta::TimeStep ts) override {
    m_CameraController.OnUpdate(ts); 
    ForgeDelta::Renderer::BeginScene(m_CameraController.GetCamera());

    {
      auto& BasicShader = m_ShaderLibrary.Get("BasicShader");
      ForgeDelta::Renderer::Submit(m_VertexArrayData, BasicShader, m_model);

      auto& TextureShader2 = m_ShaderLibrary.Get("Texture");
      ForgeDelta::OpenGLShaderService::BindShader(TextureShader2);
      ForgeDelta::g_TextureSystem.BindTexture(m_CheckBoardTextureID);
      ForgeDelta::OpenGLShaderService::UploadUniformInt(TextureShader2, "texture1", 0);
      ForgeDelta::Renderer::Submit(m_CheckBoardVAO, TextureShader2, m_model);

      auto& TextureShader = m_ShaderLibrary.Get("Texture");
      ForgeDelta::OpenGLShaderService::BindShader(TextureShader);
      ForgeDelta::g_TextureSystem.BindTexture(m_LogoTextureID);
      ForgeDelta::OpenGLShaderService::UploadUniformInt(TextureShader, "texture1", 0);
      ForgeDelta::Renderer::Submit(m_LogoVertexArrayData, TextureShader, m_model);
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
  VAO m_VertexArrayData;
  VBO m_VertexBufferData;
  EBO m_IndexBufferData;

  VAO m_LogoVertexArrayData;
  VBO m_LogoVertexBufferData;
  EBO m_LogoIndexBufferData;

  VAO m_CheckBoardVAO;
  VBO m_CheckBoardVBO;
  EBO m_CheckBoardEBO;

  uint32_t m_LogoTextureID;
  uint32_t m_CheckBoardTextureID;

  glm::mat4 m_model{ 1.0f };

  ForgeDelta::OrthographicCamera2DController m_CameraController{ 16.0f / 9.0f, true };

  private:

    void SetupBuffers(VAO& vao, VBO& vbo, EBO& ebo,GLfloat* vertices, size_t verticesSize,
      GLuint* indices, size_t indicesSize, const ForgeDelta::BufferLayout& layout) {
      ForgeDelta::OpenGLVertexArrayService::CreateVertexArray(vao);

      vbo.Data = vertices;
      vbo.Size = verticesSize;
      vbo.Layout = layout;
      ForgeDelta::OpenGLBufferService::CreateVertexBuffer(vbo);

      ebo.Data = indices;
      ebo.Size = indicesSize;
      ebo.Count = indicesSize / sizeof(GLuint);
      ForgeDelta::OpenGLBufferService::CreateIndexBuffer(ebo);

      ForgeDelta::OpenGLVertexArrayService::AddVertexBuffer(vao, &vbo);
      ForgeDelta::OpenGLVertexArrayService::SetIndexBuffer(vao, &ebo);
    }
};




class SandBox2D : public ForgeDelta::Layer {
public:
  SandBox2D() : Layer("SandBox2D") {}
  virtual ~SandBox2D() = default;

  void OnAttach() override {
    FD_PROFILE_FUNCTION(); // Profile the OnAttach function
    ForgeDelta::Renderer2D::Init();
  }

  void OnDetach() override {
    FD_PROFILE_FUNCTION(); // Profile the OnDetach function
    ForgeDelta::Renderer2D::Shutdown();
  }

  void OnUpdate(ForgeDelta::TimeStep ts) override {
    FD_PROFILE_FUNCTION(); // Profile the OnUpdate function

    // Update
    {
      FD_PROFILE_SCOPE("SandBox2D::OnUpdate - CameraController Update");
      m_CameraController.OnUpdate(ts);
    }

    ForgeDelta::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    ForgeDelta::RenderCommand::Clear();

    ForgeDelta::Renderer2D::BeginScene(m_CameraController.GetCamera());

    // Render
    {
      FD_PROFILE_SCOPE("SandBox2D::OnUpdate - Render");

      // Example usage of Renderer2D to draw quads
      ForgeDelta::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red Quad
      ForgeDelta::Renderer2D::DrawQuad(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green Quad
      ForgeDelta::Renderer2D::DrawQuad(glm::vec3(0.0f, 1.0f, -1.f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue Quad

      // Texture
      ForgeDelta::Renderer2D::DrawQuad(glm::vec2(1.0f, 1.0f), glm::vec2(10.f, 10.f), m_CheckerBoardTextureID);
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

    ImGui::Text("Profiling Results:");
    const auto& data = ForgeDelta::Application::Get().GetPerformanceProfiler().GetPerFrameData();
    for (const auto& result : data) {
      ImGui::Text("%s: %f ms ", result.first.c_str(), result.second.Time / result.second.Samples);
    }

    ImGui::End();
  }

private:
  ForgeDelta::OrthographicCamera2DController m_CameraController{ 16.0f / 9.0f, true };
  glm::vec4 uColor{ 0.2f, 0.3f, 0.8f, 1.0f };

  uint32_t m_CheckerBoardTextureID = ForgeDelta::g_TextureSystem.CreateTexture2D("assets/textures/CheckerBoard.png");
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