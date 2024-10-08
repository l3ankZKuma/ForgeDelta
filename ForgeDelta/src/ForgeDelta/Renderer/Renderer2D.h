#pragma once

#include "ForgeDelta/Core/Base.h"
#include "ForgeDelta/Renderer/Camera/Orthographic2DCamera.h"
#include "ForgeDelta/Renderer/Texture.h"

namespace ForgeDelta {

  class Renderer2D
  {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Orthographic2DCamera& camera);
    static void EndScene();

    // Primitives
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t subtextureID, bool sub = false, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, uint32_t subtextureID, bool sub = false, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


     
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, uint32_t textureID, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, uint32_t textureID, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

    // Stats
    struct Statistics
    {
      uint32_t DrawCalls = 0;
      uint32_t QuadCount = 0;

      uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
      uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
    };
    static void ResetStats();
    static Statistics GetStats();


  private:
    static void FlushAndReset();
    static void Flush();

  };

}
