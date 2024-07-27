#pragma once

#include"glm/glm.hpp"

namespace ForgeDelta {
	class Orthographic2DCamera;
	struct VAO_Data;
	struct ShaderData;
}

namespace ForgeDelta {


  class Renderer {
  public:
    static void Init();
    static void OnWindowResize(uint32_t width, uint32_t height);
    static void Shutdown();

    static void Submit(VAO_Data& VAO, ShaderData& shader, const glm::mat4& transform);

    static void BeginScene(const Orthographic2DCamera& camera);
    static void EndScene();

  private:
    struct SceneData {
      glm::mat4 ViewProjectionMatrix;
    };

    inline static SceneData* s_SceneData{ new SceneData };
  };

}
