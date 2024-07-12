#pragma once

#include"RendererAPI.h"

namespace ForgeDelta {
  struct VertexArrayData;
}

namespace ForgeDelta {

  class RenderCommand {
  public:

    static void Init() {
      RendererAPI::Init();
    }

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
      RendererAPI::SetViewport(x, y, width, height);
    }

    static void SetClearColor(const glm::vec4& color) {
      RendererAPI::SetClearColor(color);
    }

    static void Clear() {
      RendererAPI::Clear();
    }

    static void DrawIndexed(VertexArrayData& vertexArray) {
      RendererAPI::DrawIndexed(vertexArray);
    }

  };
}
