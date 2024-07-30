#include "fdpch.h"
#include "Renderer2D.h"
#include "ForgeDelta/Renderer/Shader.h"
#include "ForgeDelta/Renderer/Texture.h"
#include "ForgeDelta/Renderer/VertexArray.h"
#include "ForgeDelta/Renderer/Buffer.h"
#include "ForgeDelta/Renderer/Camera/Orthographic2DCamera.h"
#include "ForgeDelta/Renderer/RenderCommand.h"
#include "ForgeDelta/Core/Log.h"
#include "ForgeDelta/Debug/Instrumentor.h"

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
    static const uint32_t MaxQuads = 20000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

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

    Renderer2D::Statistics Stats;
  };

  static Renderer2DStorage s_Data;
  static ShaderLibrary s_ShaderLibrary;


  void Renderer2D::Init() {
    FD_PROFILE_FUNCTION();

    // Load the Quad shader
    s_ShaderLibrary.Load("Texture", "assets/shaders/Texture.glsl");
    s_Data.QuadShader = s_ShaderLibrary.Get("Texture");

    // White Texture
    uint32_t whiteTexture = g_TextureSystem.CreateTexture2D(1, 1, 0xffffffff);
    s_Data.TextureSlots[0] = whiteTexture;

    // Create and bind VAO first
    VAOSystem::CreateVertexArray(s_Data.QuadVAO);
    VAOSystem::BindVertexArray(s_Data.QuadVAO);

    // VBO
    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" },
        { ShaderDataType::Float2, "a_TexCoord" },
        { ShaderDataType::Float, "a_TexIndex" },
        { ShaderDataType::Float, "a_TilingFactor" }
    };

    s_Data.QuadVBO = { nullptr, sizeof(QuadVertex) * s_Data.MaxVertices, layout };
    BufferSystem::CreateVBO(s_Data.QuadVBO, true);  // Use dynamic buffer

    // Add VBO to VAO
    VAOSystem::AddVertexBuffer(s_Data.QuadVAO, &s_Data.QuadVBO);

    // Create and set up the quad vertex buffer
    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

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
    s_Data.QuadEBO = { quadIndices, sizeof(uint32_t) * s_Data.MaxIndices, s_Data.MaxIndices };
    BufferSystem::CreateEBO(s_Data.QuadEBO);

    // Add EBO to VAO
    VAOSystem::SetIndexBuffer(s_Data.QuadVAO, &s_Data.QuadEBO);

    delete[] quadIndices;

    // Unbind VAO
    VAOSystem::UnbindVertexArray();

    // Set up shader uniforms
    ShaderSystem::BindShader(s_Data.QuadShader);
    int samplers[s_Data.MaxTextureSlots];
    for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
      samplers[i] = i;
    ShaderSystem::UploadUniformIntArray(s_Data.QuadShader, "u_Textures", samplers, s_Data.MaxTextureSlots);

    // Set up vertex positions
    s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

  }

  void Renderer2D::Shutdown() {
    FD_PROFILE_FUNCTION();


    delete[] s_Data.QuadVertexBufferBase;
    BufferSystem::DeleteVBO(s_Data.QuadVBO);  
    BufferSystem::DeleteEBO(s_Data.QuadEBO);
    VAOSystem::DeleteVertexArray(s_Data.QuadVAO);
  }



  void Renderer2D::BeginScene(const Orthographic2DCamera& camera) {
    FD_PROFILE_FUNCTION();

    ShaderSystem::BindShader(s_Data.QuadShader);
    ShaderSystem::UploadUniformMat4(s_Data.QuadShader, "u_ViewProjection", camera.GetViewProjectionMatrix());

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
    s_Data.TextureSlotIndex = 1;
  }

  void Renderer2D::EndScene() {
    FD_PROFILE_FUNCTION();
    Flush();
  }

  void Renderer2D::Flush() {
    if (s_Data.QuadIndexCount == 0)
      return; // Nothing to draw

    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
    BufferSystem::SetVBO(s_Data.QuadVBO, s_Data.QuadVertexBufferBase, dataSize);

    // Bind textures
    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
      g_TextureSystem.BindTexture(s_Data.TextureSlots[i], i);

    RenderCommand::DrawIndexed(s_Data.QuadVAO, s_Data.QuadIndexCount);
    s_Data.Stats.DrawCalls++;
  }

  void Renderer2D::FlushAndReset() {
    EndScene();
    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
    s_Data.TextureSlotIndex = 1;
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    FD_PROFILE_FUNCTION();


    constexpr size_t quadVertexCount = 4;
    constexpr float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    constexpr float tilingFactor = 1.0f;

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
      FlushAndReset();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
      * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < quadVertexCount; i++) {
      s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = color;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
      s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
      s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
    s_Data.Stats.QuadCount++;
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID,bool sub, float tilingFactor, const glm::vec4& tintColor) {
    DrawQuad({ position.x, position.y, 0.0f }, size, textureID, sub,tilingFactor, tintColor);
  }


  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, uint32_t textureID, bool sub,float tilingFactor, const glm::vec4& tintColor) {
    FD_PROFILE_FUNCTION();

    constexpr size_t quadVertexCount = 4;

    // Default texture coordinates for a quad
    constexpr glm::vec2 defaultTextureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const glm::vec2* textureCoords = defaultTextureCoords;

    // If subtexture, get the subtexture coordinates
    if (sub) {
      textureCoords = g_TextureSystem.GetSubTextureCoords(textureID);
      if (!textureCoords) {
        // Handle error if subtexture coordinates are not valid
        FD_CORE_ERROR("Invalid subtexture coordinates for texture ID: {0}", textureID);
        return;
      }
    }

    // Check if we need to flush and reset
    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices) {
      FlushAndReset();
    }

    // Check if the texture is already in the texture slot
    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
      if (s_Data.TextureSlots[i] == textureID) {
        textureIndex = static_cast<float>(i);
        break;
      }
    }

    // If the texture is not already in the texture slot, add it
    if (textureIndex == 0.0f) {
      if (s_Data.TextureSlotIndex >= Renderer2DStorage::MaxTextureSlots) {
        FlushAndReset();
      }

      textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
      s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
      s_Data.TextureSlotIndex++;
    }

    // Precompute the transformation matrix
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
      glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    // Fill the vertex buffer
    for (size_t i = 0; i < quadVertexCount; i++) {
      s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = tintColor;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];


      s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
      s_Data.QuadVertexBufferPtr++;
    }

    // Update the index count and quad count
    s_Data.QuadIndexCount += 6;
    s_Data.Stats.QuadCount++;
  }







  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
    FD_PROFILE_FUNCTION();

    constexpr size_t quadVertexCount = 4;
    const float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    constexpr float tilingFactor = 1.0f;

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
      FlushAndReset();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
      * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
      * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < quadVertexCount; i++) {
      s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = color;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
      s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
      s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
    s_Data.Stats.QuadCount++;
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, uint32_t textureID, float tilingFactor, const glm::vec4& tintColor) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, textureID, tilingFactor, tintColor);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, uint32_t textureID, float tilingFactor, const glm::vec4& tintColor) {
    FD_PROFILE_FUNCTION();

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
      FlushAndReset();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
      if (s_Data.TextureSlots[i] == textureID) {
        textureIndex = (float)i;
        break;
      }
    }

    if (textureIndex == 0.0f) {
      if (s_Data.TextureSlotIndex >= Renderer2DStorage::MaxTextureSlots) {
        FlushAndReset();
      }

      textureIndex = (float)s_Data.TextureSlotIndex;
      s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
      s_Data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
      * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
      * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < quadVertexCount; i++) {
      s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = tintColor;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
      s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
      s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
    s_Data.Stats.QuadCount++;
  }

  void Renderer2D::ResetStats() {
    memset(&s_Data.Stats, 0, sizeof(Statistics));
  }

  Renderer2D::Statistics Renderer2D::GetStats() {
    return s_Data.Stats;
  }

}
