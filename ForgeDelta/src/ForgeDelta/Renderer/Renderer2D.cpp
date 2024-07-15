#include"fdpch.h"

#include "Renderer2D.h"
#include "ForgeDelta/Renderer/Shader.h"
#include "ForgeDelta/Renderer/Texture.h"
#include "ForgeDelta/Renderer/VertexArray.h"
#include "ForgeDelta/Renderer/Buffer.h"
#include "ForgeDelta/Renderer/Camera/Orthographic2DCamera.h"
#include "ForgeDelta/Renderer/RendererCommand.h"



namespace ForgeDelta {


  using VAO = VertexArrayData;
  using VBO = VertexBufferData;
  using EBO = IndexBufferData;

  struct Renderer2DStorage {
    VAO QuadVAO;
    VBO QuadVBO;
    EBO QuadEBO;
    ShaderData Shader;
  };

  static Renderer2DStorage* s_Data = nullptr;
  static ShaderLibrary s_ShaderLibrary;

  void Renderer2D::Init() {
    s_Data = new Renderer2DStorage();

    // Load the shader
    s_ShaderLibrary.Load("FlatColor", "assets/shaders/FlatColor.glsl");

    s_Data->Shader = s_ShaderLibrary.Get("FlatColor");

    // Setup quad vertex data
    GLfloat quadVertices[] = {
      // positions     // colors       // texCoords
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
       0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    GLuint quadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Define the layout of the vertex data
    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float3, "a_Color" },
        { ShaderDataType::Float2, "a_TexCoord" }
    };

    // Setup buffers
    s_Data->QuadVBO = { quadVertices, sizeof(quadVertices) / sizeof(float), layout };
    s_Data->QuadEBO = { quadIndices, sizeof(quadIndices) / sizeof(uint32_t), 6 };

    OpenGLVertexArrayService::CreateVertexArray(s_Data->QuadVAO);
    OpenGLBufferService::CreateVertexBuffer(s_Data->QuadVBO);
    OpenGLBufferService::CreateIndexBuffer(s_Data->QuadEBO);
    OpenGLVertexArrayService::AddVertexBuffer(s_Data->QuadVAO, &s_Data->QuadVBO);
    OpenGLVertexArrayService::SetIndexBuffer(s_Data->QuadVAO, &s_Data->QuadEBO);
  }

  void Renderer2D::Shutdown() {
    OpenGLBufferService::DeleteVertexBuffer(s_Data->QuadVBO);
    OpenGLBufferService::DeleteIndexBuffer(s_Data->QuadEBO);
    OpenGLVertexArrayService::DeleteVertexArray(s_Data->QuadVAO);
    delete s_Data;
  }

  void Renderer2D::BeginScene(const Orthographic2DCamera& camera) {
    OpenGLShaderService::BindShader(s_Data->Shader);
    // Set the camera view-projection matrix in the shader
    OpenGLShaderService::UploadUniformMat4(s_Data->Shader, "u_ViewProjection", camera.GetViewProjectionMatrix());
  }

  void Renderer2D::EndScene() {
    // Unbind the shader
    OpenGLShaderService::UnbindShader();

  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad(glm::vec3(position, 0.0f), size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    // Bind the VAO
    OpenGLVertexArrayService::BindVertexArray(s_Data->QuadVAO);

    // Upload transformation matrix and color to the shader
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    OpenGLShaderService::UploadUniformMat4(s_Data->Shader, "u_Transform", transform);
    OpenGLShaderService::UploadUniformFloat4(s_Data->Shader, "u_Color", color);

    // Draw call
    RenderCommand::DrawIndexed(s_Data->QuadVAO);
  }

}