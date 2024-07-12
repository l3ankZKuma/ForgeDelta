#include "fdpch.h"
#include "VertexArray.h"
#include "ForgeDelta/Core/Log.h"

namespace ForgeDelta {

  void OpenGLVertexArrayService::CreateVertexArray(VertexArrayData& vertexArrayData) {
    glGenVertexArrays(1, &vertexArrayData.VAO);
    glBindVertexArray(vertexArrayData.VAO);
  }

  void OpenGLVertexArrayService::DeleteVertexArray(VertexArrayData& vertexArrayData) {
    glDeleteVertexArrays(1, &vertexArrayData.VAO);
  }

  void OpenGLVertexArrayService::BindVertexArray(const VertexArrayData& vertexArrayData) {
    glBindVertexArray(vertexArrayData.VAO);
  }

  void OpenGLVertexArrayService::UnbindVertexArray() {
    glBindVertexArray(0);
  }

  void OpenGLVertexArrayService::AddVertexBuffer(VertexArrayData& vertexArrayData, VertexBufferData* vertexBufferData) {
    glBindVertexArray(vertexArrayData.VAO);
    OpenGLBufferService::BindVertexBuffer(*vertexBufferData);

    const auto& layout = vertexBufferData->Layout;
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

    vertexArrayData.VertexBuffers.push_back(vertexBufferData);
  }

  void OpenGLVertexArrayService::SetIndexBuffer(VertexArrayData& vertexArrayData, IndexBufferData* indexBufferData) {
    glBindVertexArray(vertexArrayData.VAO);
    OpenGLBufferService::BindIndexBuffer(*indexBufferData);

    vertexArrayData.IndexBuffer = indexBufferData;
  }

  GLenum OpenGLVertexArrayService::ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
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