#include"fdpch.h"
#include "Buffer.h"

namespace ForgeDelta {

  // Shader Data Type Size Function
  uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float: return 4;
    case ShaderDataType::Float2: return 4 * 2;
    case ShaderDataType::Float3: return 4 * 3;
    case ShaderDataType::Float4: return 4 * 4;
    case ShaderDataType::Mat3: return 4 * 3 * 3;
    case ShaderDataType::Mat4: return 4 * 4 * 4;
    case ShaderDataType::Int: return 4;
    case ShaderDataType::Int2: return 4 * 2;
    case ShaderDataType::Int3: return 4 * 3;
    case ShaderDataType::Int4: return 4 * 4;
    case ShaderDataType::Bool: return 1;
    }
    return 0;
  }

  // Calculate Offsets and Stride for Buffer Layout
  void BufferLayout::CalculateOffsetsAndStride() {
    size_t offset = 0;
    Stride = 0;
    for (auto& element : Elements) {
      element.Offset = offset;
      offset += element.Size;
      Stride += element.Size;
    }
  }

  // Get Component Count for Buffer Element
  uint32_t BufferElement::GetComponentCount() const {
    switch (Type) {
    case ShaderDataType::Float: return 1;
    case ShaderDataType::Float2: return 2;
    case ShaderDataType::Float3: return 3;
    case ShaderDataType::Float4: return 4;
    case ShaderDataType::Mat3: return 3;
    case ShaderDataType::Mat4: return 4;
    case ShaderDataType::Int: return 1;
    case ShaderDataType::Int2: return 2;
    case ShaderDataType::Int3: return 3;
    case ShaderDataType::Int4: return 4;
    case ShaderDataType::Bool: return 1;
    }
    return 0;
  }

  // OpenGL Buffer Service Implementations
  void OpenGLBufferService::CreateVertexBuffer(VertexBufferData& bufferData) {
    glGenBuffers(1, &bufferData.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, bufferData.VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferData.Size * sizeof(GLfloat), bufferData.Data, GL_STATIC_DRAW);
  }

  void OpenGLBufferService::DeleteVertexBuffer(VertexBufferData& bufferData) {
    glDeleteBuffers(1, &bufferData.VBO);
  }

  void OpenGLBufferService::BindVertexBuffer(const VertexBufferData& bufferData) {
    glBindBuffer(GL_ARRAY_BUFFER, bufferData.VBO);
  }

  void OpenGLBufferService::UnbindVertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void OpenGLBufferService::CreateIndexBuffer(IndexBufferData& bufferData) {
    glGenBuffers(1, &bufferData.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferData.Size * sizeof(GLuint), bufferData.Data, GL_STATIC_DRAW);
  }

  void OpenGLBufferService::DeleteIndexBuffer(IndexBufferData& bufferData) {
    glDeleteBuffers(1, &bufferData.EBO);
  }

  void OpenGLBufferService::BindIndexBuffer(const IndexBufferData& bufferData) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData.EBO);
  }

  void OpenGLBufferService::UnbindIndexBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

}
