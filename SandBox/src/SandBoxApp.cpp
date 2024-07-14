#include <ForgeDelta.h>
#include "imgui.h"

using VAO = ForgeDelta::VertexArrayData;
using VBO = ForgeDelta::VertexBufferData;
using EBO = ForgeDelta::IndexBufferData;

class Layers : public ForgeDelta::Layer {
public:
  Layers() : Layer("Layers") {}

  void OnAttach() override {
    // Load the shader from the file
    m_ShaderLibrary.Load("BasicShader", "assets/shaders/BasicShader.glsl");
    m_ShaderLibrary.Load("Texture", "assets/shaders/Texture.glsl");

    GLfloat vertices[] = {
      // Position      // Color
      -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom-left vertex (red)
       0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-right vertex (green)
       0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top vertex (blue)
    };

    GLuint indices[] = {
        0, 1, 2
    };

    // Vertex Array
    ForgeDelta::OpenGLVertexArrayService::CreateVertexArray(m_VertexArrayData);

    // Vertex Buffer
    m_VertexBufferData.Data = vertices;
    m_VertexBufferData.Size = sizeof(vertices);
    m_VertexBufferData.Layout = {
        { ForgeDelta::ShaderDataType::Float3, "a_Position" },
        { ForgeDelta::ShaderDataType::Float3, "a_Color" }
    };
    ForgeDelta::OpenGLBufferService::CreateVertexBuffer(m_VertexBufferData);

    // Index Buffer
    m_IndexBufferData.Data = indices;
    m_IndexBufferData.Size = sizeof(indices);
    m_IndexBufferData.Count = 3;
    ForgeDelta::OpenGLBufferService::CreateIndexBuffer(m_IndexBufferData);

    // Add buffers to vertex array
    ForgeDelta::OpenGLVertexArrayService::AddVertexBuffer(m_VertexArrayData, &m_VertexBufferData);
    ForgeDelta::OpenGLVertexArrayService::SetIndexBuffer(m_VertexArrayData, &m_IndexBufferData);

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

    // Vertex Array
    ForgeDelta::OpenGLVertexArrayService::CreateVertexArray(m_LogoVertexArrayData);

    m_LogoVertexBufferData.Data = logoVertices;
    m_LogoVertexBufferData.Size = sizeof(logoVertices);
    m_LogoVertexBufferData.Layout = {
        { ForgeDelta::ShaderDataType::Float3, "a_Position" },
        { ForgeDelta::ShaderDataType::Float3, "a_Color" },
        { ForgeDelta::ShaderDataType::Float2, "a_TexCoord" }
    };
    ForgeDelta::OpenGLBufferService::CreateVertexBuffer(m_LogoVertexBufferData);

    m_LogoIndexBufferData.Data = logoIndices;
    m_LogoIndexBufferData.Size = sizeof(logoIndices);
    m_LogoIndexBufferData.Count = 6;
    ForgeDelta::OpenGLBufferService::CreateIndexBuffer(m_LogoIndexBufferData);

    ForgeDelta::OpenGLVertexArrayService::AddVertexBuffer(m_LogoVertexArrayData, &m_LogoVertexBufferData);
    ForgeDelta::OpenGLVertexArrayService::SetIndexBuffer(m_LogoVertexArrayData, &m_LogoIndexBufferData);

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

    ForgeDelta::OpenGLVertexArrayService::CreateVertexArray(m_CheckBoardVAO);

    m_CheckBoardVBO.Data = checkerVertices;
    m_CheckBoardVBO.Size = sizeof(checkerVertices);
    m_CheckBoardVBO.Layout = {
        { ForgeDelta::ShaderDataType::Float3, "a_Position" },
        { ForgeDelta::ShaderDataType::Float3, "a_Color" },
        { ForgeDelta::ShaderDataType::Float2, "a_TexCoord" }
    };
    ForgeDelta::OpenGLBufferService::CreateVertexBuffer(m_CheckBoardVBO);

    m_CheckBoardEBO.Data = checkerIndices;
    m_CheckBoardEBO.Size = sizeof(checkerIndices);
    m_CheckBoardEBO.Count = 6;
    ForgeDelta::OpenGLBufferService::CreateIndexBuffer(m_CheckBoardEBO);

    ForgeDelta::OpenGLVertexArrayService::AddVertexBuffer(m_CheckBoardVAO, &m_CheckBoardVBO);
    ForgeDelta::OpenGLVertexArrayService::SetIndexBuffer(m_CheckBoardVAO, &m_CheckBoardEBO);

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
};

class SandBox : public ForgeDelta::Application {
public:
  SandBox() {
    PushLayer(new Layers());
  }

  ~SandBox() {}
};

ForgeDelta::Application* ForgeDelta::CreateApplication() {
  return new SandBox();
}