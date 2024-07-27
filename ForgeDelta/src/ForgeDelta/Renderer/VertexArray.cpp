#include "fdpch.h"
#include "VertexArray.h"
#include "ForgeDelta/Core/Log.h"

namespace ForgeDelta {

  void VAOSystem::CreateVertexArray(VAO_Data& VAO_Data) {
    glGenVertexArrays(1, &VAO_Data.VAO);
    glBindVertexArray(VAO_Data.VAO);
  }

  void VAOSystem::DeleteVertexArray(VAO_Data& VAO_Data) {
    glDeleteVertexArrays(1, &VAO_Data.VAO);
  }

  void VAOSystem::BindVertexArray(const VAO_Data& VAO_Data) {
    glBindVertexArray(VAO_Data.VAO);
  }


  void VAOSystem::AddVertexBuffer(VAO_Data& VAO_Data, VBO_Data* VBO_Data) {
    glBindVertexArray(VAO_Data.VAO);
    BufferSystem::BindVBO(*VBO_Data);

    const auto& layout = VBO_Data->Layout;
    const auto& elements = layout.Elements;
    uint32_t index = 0;
    for (const auto& element : elements) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index,
        element.GetComponentCount(),
        ShaderDataTypeToOpenGLBaseType(element.Type),
        element.Normalized ? GL_TRUE : GL_FALSE,
        layout.Stride,
        (const void*)element.Offset);
      index++;
    }

    VAO_Data.VertexBuffers.push_back(VBO_Data);
  }

  void VAOSystem::SetIndexBuffer(VAO_Data& VAO_Data, EBO_Data* EBO_Data) {
    glBindVertexArray(VAO_Data.VAO);
    BufferSystem::BindEBO(*EBO_Data);

    VAO_Data.IndexBuffer = EBO_Data;
  }

  GLenum VAOSystem::ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float: return GL_FLOAT;
    case ShaderDataType::Float2: return GL_FLOAT;
    case ShaderDataType::Float3: return GL_FLOAT;
    case ShaderDataType::Float4: return GL_FLOAT;
    case ShaderDataType::Mat3: return GL_FLOAT;
    case ShaderDataType::Mat4: return GL_FLOAT;
    case ShaderDataType::Int: return GL_INT;
    case ShaderDataType::Int2: return GL_INT;
    case ShaderDataType::Int3: return GL_INT;
    case ShaderDataType::Int4: return GL_INT;
    case ShaderDataType::Bool: return GL_BOOL;
    default: FD_CORE_ERROR("Unknown ShaderDataType!"); return 0;
    }
  }

} // namespace ForgeDelta