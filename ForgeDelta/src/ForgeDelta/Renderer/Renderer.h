#pragma once

#include"glm/glm.hpp"

namespace ForgeDelta {
	class Orthographic2DCamera;
	struct VertexArrayData;
	struct ShaderData;
}

namespace ForgeDelta {

	class  Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Shutdown();

		static void Submit(VertexArrayData & VAO,const ShaderData &shader); 

		static void BeginScene(/*const Orthographic2DCamera& camera*/);
		static void EndScene();



	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		inline static SceneData* s_SceneData = new SceneData;

	};

}
