#pragma once

#include "Buffer.h"

namespace ForgeDelta {

  struct VAO_Data {
    GLuint VAO;
    std::vector<VBO_Data*> VertexBuffers;
    EBO_Data* IndexBuffer;
  };

  class VAOSystem {
  public:
    static void CreateVertexArray(VAO_Data& VAO_Data);
    static void DeleteVertexArray(VAO_Data& VAO_Data);
    static void BindVertexArray(const VAO_Data& VAO_Data);
    inline static void UnbindVertexArray() { glBindVertexArray(0); }

    static void AddVertexBuffer(VAO_Data& VAO_Data, VBO_Data* VBO_Data);
    static void SetIndexBuffer(VAO_Data& VAO_Data, EBO_Data* EBO_Data);

  private:
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
  };

} // namespace ForgeDelta