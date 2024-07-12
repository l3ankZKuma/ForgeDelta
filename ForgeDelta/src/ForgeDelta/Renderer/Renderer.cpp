#include "fdpch.h"
#include "Renderer.h"


#include "ForgeDelta/Renderer/RendererCommand.h"
#include "ForgeDelta/Renderer/VertexArray.h"
#include"ForgeDelta/Renderer/Shader.h"


namespace ForgeDelta {
  void Renderer::Init() {
    RenderCommand::Init();
  }

  //void Renderer::BeginScene(const Orthographic2DCamera& camera) {

  //  s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

  //}

  void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
  }

  void Renderer::EndScene() {

  }

  void Renderer::Submit(VertexArrayData & VAO,const ShaderData & shader) {


    OpenGLVertexArrayService::BindVertexArray(VAO);
    RenderCommand::DrawIndexed(VAO);
    OpenGLVertexArrayService::UnbindVertexArray();
    OpenGLShaderService::UnbindShader();


  }
}