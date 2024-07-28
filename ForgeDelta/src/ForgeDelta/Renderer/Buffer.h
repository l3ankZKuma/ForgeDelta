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
  struct VBO_Data {
    GLfloat* Data;
    GLuint Size;
    BufferLayout Layout;
    GLuint VBO;
  };

  // Index Buffer Data Struct
  struct EBO_Data {
    GLuint* Data;
    GLuint Size;
    GLuint Count;
    GLuint EBO;
  };

  // OpenGL Buffer Service
  class BufferSystem {
  public:
    static void CreateVBO(VBO_Data& bufferData,bool isStatic = true);
    static void DeleteVBO(VBO_Data& bufferData);
    static void BindVBO(const VBO_Data& bufferData);
    static void UnBindVBO();
    static void SetVBO(VBO_Data& bufferData,const void*data ,uint32_t size);

    static void CreateEBO(EBO_Data& bufferData);
    static void DeleteEBO(EBO_Data& bufferData);
    static void BindEBO(const EBO_Data& bufferData);
    static void UnBindEBO();
  };



}