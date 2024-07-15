#pragma once

#include "ForgeDelta/Core/Base.h"

namespace ForgeDelta {

  class Orthographic2DCamera;
  class TextureSystem;

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
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, uint32_t textureID);

  private:
    static void SetupQuad();
  };

}