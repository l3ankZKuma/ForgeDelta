#pragma once

#include "Buffer.h"

namespace ForgeDelta {

  struct VertexArrayData {
    GLuint VAO;
    std::vector<VertexBufferData*> VertexBuffers;
    IndexBufferData* IndexBuffer;
  };

  class OpenGLVertexArrayService {
  public:
    static void CreateVertexArray(VertexArrayData& vertexArrayData);
    static void DeleteVertexArray(VertexArrayData& vertexArrayData);
    static void BindVertexArray(const VertexArrayData& vertexArrayData);
    inline static void UnbindVertexArray(){glBindVertexArray(0);}

    static void AddVertexBuffer(VertexArrayData& vertexArrayData, VertexBufferData* vertexBufferData);
    static void SetIndexBuffer(VertexArrayData& vertexArrayData, IndexBufferData* indexBufferData);

  private:
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
  };

} // namespace ForgeDelta