#include "fdpch.h"
#include "Renderer.h"


#include "ForgeDelta/Renderer/RenderCommand.h"
#include "ForgeDelta/Renderer/VertexArray.h"
#include"ForgeDelta/Renderer/Shader.h"
#include"ForgeDelta/Renderer/RenderCommandQueue.h"
#include"ForgeDelta/Renderer/Camera/Orthographic2DCamera.h"


namespace ForgeDelta {

  Renderer *Renderer::s_Instance = new Renderer();

  void Renderer::Init() {
    RenderCommand::Init();
  }

  void Renderer::Clear()
  {
  }

  void Renderer::Clear(float r, float g, float b, float a)
  {
    float params[4] = { r, g, b, a };
    s_Instance->m_CommandQueue.SubmitCommand(RenderCommand::Clear, params, sizeof(float) * 4);
  }



  void Renderer::BeginScene(const Orthographic2DCamera& camera) {
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
  }

  void Renderer::EndScene() {}

  void Renderer::WaitAndRender() {
    s_Instance->m_CommandQueue.Execute();
  }


}