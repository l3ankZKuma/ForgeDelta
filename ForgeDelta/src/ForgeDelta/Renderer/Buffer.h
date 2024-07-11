#pragma once

#include"ForgeDelta/Core/Log.h"
#include <glad/glad.h>

namespace ForgeDelta {



  enum class ShaderDataType {
    None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
  };

  // Helper functions
  uint32_t ShaderDataTypeSize(ShaderDataType type);

  // Buffer Element Struct
  struct BufferElement {
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    size_t Offset;
    bool Normalized;

    BufferElement() = default;
    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
      : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

    uint32_t GetComponentCount() const;
  };

  // Buffer Layout Struct
  struct BufferLayout {
    std::vector<BufferElement> Elements;
    uint32_t Stride;

    BufferLayout() : Stride(0) {}
    BufferLayout(std::initializer_list<BufferElement> elements)
      : Elements(elements), Stride(0) {
      CalculateOffsetsAndStride();
    }

    void CalculateOffsetsAndStride();
  };

  // Vertex Buffer Data Struct
  struct VertexBufferData {
    GLfloat* Data;
    GLuint Size;
    BufferLayout Layout;
    GLuint VBO;
  };

  // Index Buffer Data Struct
  struct IndexBufferData {
    GLuint* Data;
    GLuint Size;
    GLuint Count;
    GLuint EBO;
  };

  // OpenGL Buffer Service
  class OpenGLBufferService {
  public:
    static void CreateVertexBuffer(VertexBufferData& bufferData);
    static void DeleteVertexBuffer(VertexBufferData& bufferData);
    static void BindVertexBuffer(const VertexBufferData& bufferData);
    static void UnbindVertexBuffer();

    static void CreateIndexBuffer(IndexBufferData& bufferData);
    static void DeleteIndexBuffer(IndexBufferData& bufferData);
    static void BindIndexBuffer(const IndexBufferData& bufferData);
    static void UnbindIndexBuffer();
  };



}
