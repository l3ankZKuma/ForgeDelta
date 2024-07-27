#include "fdpch.h"
#include "Renderer2D.h"
#include "ForgeDelta/Renderer/Shader.h"
#include "ForgeDelta/Renderer/Texture.h"
#include "ForgeDelta/Renderer/VertexArray.h"
#include "ForgeDelta/Renderer/Buffer.h"
#include "ForgeDelta/Renderer/Camera/Orthographic2DCamera.h"
#include "ForgeDelta/Renderer/RendererCommand.h"



namespace ForgeDelta {

  using VAO = VAO_Data;
  using VBO = VBO_Data;
  using EBO = EBO_Data;

  struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
  };


struct Renderer2DStorage {
    static constexpr uint32_t MaxQuads = 10000;
    static constexpr uint32_t MaxVertices = MaxQuads * 4;
    static constexpr uint32_t MaxIndices = MaxQuads * 6;
    static constexpr uint32_t MaxTextureSlots = 32;

    VAO_Data QuadVAO;
    VBO QuadVBO;
    EBO QuadEBO;
    ShaderData QuadShader;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    std::array<uint32_t, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 is reserved for white texture

    glm::vec4 QuadVertexPositions[4];
};

  static  Renderer2DStorage s_Data;
  static ShaderLibrary s_ShaderLibrary;

  void Renderer2D::Init() {
    // Load the Quad shader
    s_ShaderLibrary.Load("Texture", "assets/shaders/Texture.glsl");
    s_Data.QuadShader = s_ShaderLibrary.Get("Texture");

    //White Texture
    uint32_t whiteTexture = g_TextureSystem.CreateTexture2D(1, 1, 0xffffffff);
    s_Data.TextureSlots[0] = whiteTexture;



    // Create and set up the quad vertex buffer
    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    // Set up indices
    uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
      quadIndices[i + 0] = offset + 0;
      quadIndices[i + 1] = offset + 1;
      quadIndices[i + 2] = offset + 2;

      quadIndices[i + 3] = offset + 2;
      quadIndices[i + 4] = offset + 3;
      quadIndices[i + 5] = offset + 0;

      offset += 4;
    }

    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" },
        { ShaderDataType::Float2, "a_TexCoord" }
        /*,
        { ShaderDataType::Float, "a_TexIndex" },
        { ShaderDataType::Float, "a_TilingFactor" }*/
    };

    s_Data.QuadVBO = { nullptr, sizeof(QuadVertex)*s_Data.MaxVertices,layout };
    BufferSystem::CreateVBO(s_Data.QuadVBO,false);

    s_Data.QuadEBO = { quadIndices, sizeof(quadIndices),s_Data.MaxIndices};
    BufferSystem::CreateEBO(s_Data.QuadEBO);


    VAOSystem::CreateVertexArray(s_Data.QuadVAO);
    VAOSystem::AddVertexBuffer(s_Data.QuadVAO, &s_Data.QuadVBO);
    VAOSystem::SetIndexBuffer(s_Data.QuadVAO, &s_Data.QuadEBO);

    delete[] quadIndices;
  }


  void Renderer2D::Shutdown() {
    BufferSystem::DeleleVBO(s_Data.QuadVBO);
    BufferSystem::DeleteEBO(s_Data.QuadEBO);
    VAOSystem::DeleteVertexArray(s_Data.QuadVAO);
  }

  void Renderer2D::BeginScene(const Orthographic2DCamera& camera) {
    ShaderSystem::BindShader(s_Data.QuadShader);
    ShaderSystem::UploadUniformMat4(s_Data.QuadShader, "u_ViewProjection", camera.GetViewProjectionMatrix());
  
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

  }

  void Renderer2D::EndScene() {

    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
    BufferSystem::SetVBO(s_Data.QuadVBO, s_Data.QuadVertexBufferBase, dataSize);

    ShaderSystem::UnbindShader();
    Flush();
  }

  void Renderer2D::Flush() {

    /*
    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
      g_TextureSystem.BindTexture(s_Data.TextureSlots[i], i);
    }
    */

    RenderCommand::DrawIndexed(s_Data.QuadVAO, s_Data.QuadIndexCount);
    s_Data.QuadIndexCount = 0;
    s_Data.TextureSlotIndex = 1;


  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad(glm::vec3(position, 0.0f), size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
   
    g_TextureSystem.BindTexture(s_Data.TextureSlots[0]);
    ShaderSystem::BindShader(s_Data.QuadShader);
    ShaderSystem::UploadUniformFloat4(s_Data.QuadShader, "u_Color", color);
    ShaderSystem::UploadUniformInt(s_Data.QuadShader, "u_Texture", 0);

    s_Data.QuadVertexBufferPtr->Position = position;
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
    s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
    s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.QuadVertexBufferPtr++;
    
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID, float tilingFactor, const glm::vec4& tintColor) {
    DrawQuad(glm::vec3(position, 0.0f), size, textureID, tilingFactor, tintColor);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, uint32_t textureID, float tilingFactor, const glm::vec4& tintColor) {
    ShaderSystem::BindShader(s_Data.QuadShader);
    g_TextureSystem.BindTexture(textureID);

    VAOSystem::BindVertexArray(s_Data.QuadVAO);
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    ShaderSystem::UploadUniformMat4(s_Data.QuadShader, "u_Transform", transform);
    ShaderSystem::UploadUniformInt(s_Data.QuadShader, "u_Texture", 0);
    ShaderSystem::UploadUniformBool(s_Data.QuadShader, "u_UseTexture", true);
    ShaderSystem::UploadUniformFloat(s_Data.QuadShader, "u_TilingFactor", tilingFactor);
    ShaderSystem::UploadUniformFloat4(s_Data.QuadShader, "u_TintColor", tintColor);

    RenderCommand::DrawIndexed(s_Data.QuadVAO);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
    DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
    ShaderSystem::BindShader(s_Data.QuadShader);
    VAOSystem::BindVertexArray(s_Data.QuadVAO);

    glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    ShaderSystem::UploadUniformMat4(s_Data.QuadShader, "u_Transform", transform);
    ShaderSystem::UploadUniformFloat4(s_Data.QuadShader, "u_Color", color);
    ShaderSystem::UploadUniformBool(s_Data.QuadShader, "u_UseTexture", false);

    RenderCommand::DrawIndexed(s_Data.QuadVAO);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, uint32_t textureID, float tilingFactor, const glm::vec4& tintColor) {
    DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, textureID, tilingFactor, tintColor);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, uint32_t textureID, float tilingFactor, const glm::vec4& tintColor) {
    ShaderSystem::BindShader(s_Data.QuadShader);
    g_TextureSystem.BindTexture(textureID);

    VAOSystem::BindVertexArray(s_Data.QuadVAO);

    glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    ShaderSystem::UploadUniformMat4(s_Data.QuadShader, "u_Transform", transform);
    ShaderSystem::UploadUniformInt(s_Data.QuadShader, "u_Texture", 0);
    ShaderSystem::UploadUniformBool(s_Data.QuadShader, "u_UseTexture", true);
    ShaderSystem::UploadUniformFloat(s_Data.QuadShader, "u_TilingFactor", tilingFactor);
    ShaderSystem::UploadUniformFloat4(s_Data.QuadShader, "u_TintColor", tintColor);

    RenderCommand::DrawIndexed(s_Data.QuadVAO);
  }

}