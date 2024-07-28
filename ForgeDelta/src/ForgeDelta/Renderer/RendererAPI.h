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

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
      glViewport(x, y, width, height);
    }

    inline static void SetClearColor(const glm::vec4& color) {
      glClearColor(color.r, color.g, color.b, color.a);
    }

    inline static void Clear() {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline static void DrawIndexed(VAO_Data& vertexArray,uint32_t idxCnt =0) {
      glBindVertexArray(vertexArray.VAO);
      const auto& count = vertexArray.IndexBuffer->Count;
      const auto &cnt = idxCnt ? count : idxCnt;
      glDrawElements(GL_TRIANGLES, cnt, GL_UNSIGNED_INT, nullptr);
    }

    inline static API GetAPI() { return s_API; }

  private:
    inline static API s_API{ API::OpenGL };
  };
}