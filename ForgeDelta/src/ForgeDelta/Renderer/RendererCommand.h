#pragma once

#include"RendererAPI.h"

namespace ForgeDelta {
  struct VAO_Data;
}

namespace ForgeDelta {

  class RenderCommand {
  public:

    static void Init() {
      RendererAPI::Init();
    }

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
      RendererAPI::SetViewport(x, y, width, height);
    }

    inline static void SetClearColor(const glm::vec4& color) {
      RendererAPI::SetClearColor(color);
    }

    inline static void Clear() {
      RendererAPI::Clear();
    }

    inline static void DrawIndexed(VAO_Data& vertexArray,uint32_t idxCnt=0) {
      RendererAPI::DrawIndexed(vertexArray,idxCnt);
    }

  };
}
