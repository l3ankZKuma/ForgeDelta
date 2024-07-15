#pragma once

#include"glm/glm.hpp"
#include"glad/glad.h"

#include"ForgeDelta/Renderer/VertexArray.h"
#include"ForgeDelta/Renderer/Buffer.h"

namespace ForgeDelta {

  class RendererAPI {
  public:
    enum class API {
      None = 0,
      OpenGL = 1
    };

    static void Init() {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      

      glEnable(GL_DEPTH_TEST);
    }

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
      glViewport(x, y, width, height);
    }

    static void SetClearColor(const glm::vec4& color) {
      glClearColor(color.r, color.g, color.b, color.a);
    }

    static void Clear() {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    static void DrawIndexed(VertexArrayData& vertexArray) {
      const auto& count = vertexArray.IndexBuffer->Count;
      glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    inline static API GetAPI() { return s_API; }

  private:
    inline static API s_API{ API::OpenGL };
  };
}