#include <ForgeDelta.h>
#include "imgui.h"

class Layers : public ForgeDelta::Layer {
public:
  Layers() : Layer("Layers") {}

  void OnAttach() override {
    // Setup vertex data
    GLfloat vertices[] = {
      // positions        
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2
    };

    // Initialize vertex array
    ForgeDelta::OpenGLVertexArrayService::CreateVertexArray(m_VertexArrayData);

    // Initialize vertex buffer
    m_VertexBufferData.Data = vertices;
    m_VertexBufferData.Size = sizeof(vertices) / sizeof(vertices[0]);
    m_VertexBufferData.Layout = {
        { ForgeDelta::ShaderDataType::Float3, "a_Position" }
    };
    ForgeDelta::OpenGLBufferService::CreateVertexBuffer(m_VertexBufferData);

    // Initialize index buffer
    m_IndexBufferData.Data = indices;
    m_IndexBufferData.Size = sizeof(indices) / sizeof(indices[0]);
    m_IndexBufferData.Count = m_IndexBufferData.Size;
    ForgeDelta::OpenGLBufferService::CreateIndexBuffer(m_IndexBufferData);

    // Add vertex buffer and set index buffer to vertex array
    ForgeDelta::OpenGLVertexArrayService::AddVertexBuffer(m_VertexArrayData, &m_VertexBufferData);
    ForgeDelta::OpenGLVertexArrayService::SetIndexBuffer(m_VertexArrayData, &m_IndexBufferData);
  }

  void OnDetach() override {
    ForgeDelta::OpenGLBufferService::DeleteVertexBuffer(m_VertexBufferData);
    ForgeDelta::OpenGLBufferService::DeleteIndexBuffer(m_IndexBufferData);
    ForgeDelta::OpenGLVertexArrayService::DeleteVertexArray(m_VertexArrayData);
  }

  void OnUpdate(ForgeDelta::TimeStep ts) override {
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind vertex array and draw
    ForgeDelta::OpenGLVertexArrayService::BindVertexArray(m_VertexArrayData);
    glDrawElements(GL_TRIANGLES, m_IndexBufferData.Count, GL_UNSIGNED_INT, nullptr);
    ForgeDelta::OpenGLVertexArrayService::UnbindVertexArray();
  }

  void OnEvent(ForgeDelta::Event& e) override {
    // Handle events here
  }

  void OnImGuiRender() override {
    // ImGui rendering here
  }

private:
  ForgeDelta::VertexArrayData m_VertexArrayData;
  ForgeDelta::VertexBufferData m_VertexBufferData;
  ForgeDelta::IndexBufferData m_IndexBufferData;
};

class SandBox : public ForgeDelta::Application {
public:
  SandBox() {
    PushLayer(new Layers());
  }

  ~SandBox() {
  }
};

ForgeDelta::Application* ForgeDelta::CreateApplication() {
  return new SandBox();
}
