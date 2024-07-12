#include <ForgeDelta.h>
#include "imgui.h"


class Layers : public ForgeDelta::Layer {
public:
  Layers() : Layer("Layers") {}

  void OnAttach() override {
    // Use ShaderLibrary to load shaders
    std::cout << "Loading BasicShader..." << std::endl;
    try {
      m_ShaderLibrary.Load("BasicShader", "assets/shaders/BasicShader.glsl");
      std::cout << "BasicShader loaded successfully." << std::endl;
    }
    catch (const std::exception& e) {
      std::cerr << "Failed to load BasicShader: " << e.what() << std::endl;
      return;
    }



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
  }

  void OnDetach() override {
    ForgeDelta::OpenGLBufferService::DeleteVertexBuffer(m_VertexBufferData);
    ForgeDelta::OpenGLBufferService::DeleteIndexBuffer(m_IndexBufferData);
    ForgeDelta::OpenGLVertexArrayService::DeleteVertexArray(m_VertexArrayData);
  }

  void OnUpdate(ForgeDelta::TimeStep ts) override {

    m_CameraController.OnUpdate(ts);

    ForgeDelta::Renderer::BeginScene(m_CameraController.GetCamera());

    {

      // Clear the color buffer with a default color

      // Retrieve and bind the shader from the library
      auto& BasicShader = m_ShaderLibrary.Get("BasicShader");
      auto shaderIndex = m_ShaderLibrary.GetIndex("BasicShader");
      // Submit to the renderer
      ForgeDelta::Renderer::Submit(m_VertexArrayData, BasicShader, shaderIndex, m_model);



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
  ForgeDelta::VertexArrayData m_VertexArrayData;
  ForgeDelta::VertexBufferData m_VertexBufferData;
  ForgeDelta::IndexBufferData m_IndexBufferData;
  
  glm::mat4 m_model{1.0f};

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
