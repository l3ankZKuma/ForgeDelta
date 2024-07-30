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


    
    static unsigned int Clear(void* data)
    {
      glm::vec4* color = static_cast<glm::vec4*>(data);
      RendererAPI::Clear(color->r, color->g, color->b, color->a);
      return sizeof(glm::vec4);
    }


    inline static void DrawIndexed(VAO_Data& vertexArray,uint32_t idxCnt=0) {
      RendererAPI::DrawIndexed(vertexArray,idxCnt);
    }

  };
}
