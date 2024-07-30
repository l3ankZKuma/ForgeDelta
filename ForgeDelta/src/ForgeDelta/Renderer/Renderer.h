#pragma once

#include "RenderCommandQueue.h"
#include "glm/glm.hpp"
#include <type_traits>

namespace ForgeDelta {
	class Orthographic2DCamera;
	struct VAO_Data;
	struct ShaderData;
}

namespace ForgeDelta {

	class Renderer {
	public:

    typedef void(*RenderCommandFn)(void*);


		static void Init();
		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.f);
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Shutdown();


		static void BeginScene(const Orthographic2DCamera& camera);
		static void EndScene();

		void WaitAndRender();

		inline static Renderer& Get() { return *s_Instance; }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		RenderCommandQueue m_CommandQueue;

		inline static SceneData* s_SceneData{ new SceneData };
		static Renderer* s_Instance;
	};


}


