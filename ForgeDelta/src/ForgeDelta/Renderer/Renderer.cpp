#include "fdpch.h"
#include "Renderer.h"


#include "ForgeDelta/Renderer/RendererCommand.h"
#include "ForgeDelta/Renderer/VertexArray.h"
#include"ForgeDelta/Renderer/Shader.h"

#include"ForgeDelta/Renderer/Camera/Orthographic2DCamera.h"


namespace ForgeDelta {
  void Renderer::Init() {
    RenderCommand::Init();
  }

  void Renderer::BeginScene(const Orthographic2DCamera& camera) {
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
  }

  void Renderer::EndScene() {}

  void Renderer::Submit(VAO_Data& VAO, ShaderData& shader, const glm::mat4& transform) {
    
    ShaderSystem::BindShader(shader);
    ShaderSystem::UploadUniformMat4(shader, "model", transform);
    ShaderSystem::UploadUniformMat4(shader, "vp", s_SceneData->ViewProjectionMatrix);

    VAOSystem::BindVertexArray(VAO);
    RenderCommand::DrawIndexed(VAO);
    VAOSystem::UnbindVertexArray();
    ShaderSystem::UnbindShader();
  }
}