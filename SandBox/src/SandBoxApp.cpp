#include <ForgeDelta.h>
#include "imgui.h"
#include <iostream>

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

    // Setup vertex data
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
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
        { ForgeDelta::ShaderDataType::Float3, "a_Position" }
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
    // Clear the color buffer with a default color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Retrieve and bind the shader from the library
    try {
      const auto& shader = m_ShaderLibrary.Get("BasicShader");
      std::cout << "Binding BasicShader..." << std::endl;
      ForgeDelta::OpenGLShaderService::BindShader(shader, 0);
    }
    catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return;
    }

    ForgeDelta::OpenGLVertexArrayService::BindVertexArray(m_VertexArrayData);
    glDrawElements(GL_TRIANGLES, m_IndexBufferData.Count, GL_UNSIGNED_INT, nullptr);
    ForgeDelta::OpenGLVertexArrayService::UnbindVertexArray();
    ForgeDelta::OpenGLShaderService::UnbindShader();
  }

  void OnEvent(ForgeDelta::Event& e) override {}

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
