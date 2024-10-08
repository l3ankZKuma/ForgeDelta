#include "fdpch.h"
#include "Texture.h"

#include"ForgeDelta/Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ForgeDelta {

  TextureSystem g_TextureSystem;

  TextureSystem::TextureSystem() {
    m_Used.reset();
  }

  TextureSystem::~TextureSystem() {
    for (uint32_t textureID : m_ActiveTextures) {
      glDeleteTextures(1, &m_TexturePool[textureID].rendererID);
    }
  }

  uint32_t TextureSystem::CreateTexture2D(const char* path) {
    for (size_t i = 0; i < MaxTextures; ++i) {
      if (!m_Used[i]) {
        m_Used[i] = true;
        TextureData& texture = m_TexturePool[i];
        texture.paths[0] = path;
        texture.type = TextureType::Texture2D;
        texture.isLoaded = false;
        texture.wrap = TextureWrap::Repeat;
        texture.filter = TextureFilter::Linear;
        m_ActiveTextures.push_back(i);
        LoadTexture2D(texture);
        return i;
      }
    }
    FD_CORE_ERROR("Texture pool is full!");
    return MaxTextures;
  }

  uint32_t TextureSystem::CreateTexture2D(uint32_t width, uint32_t height, uint32_t color)
  {
    for (size_t i = 0; i < MaxTextures; ++i)
    {
      if (!m_Used[i])
      {
        m_Used[i] = true;
        TextureData& texture = m_TexturePool[i];
        texture.type = TextureType::Texture2D;
        texture.width = width;
        texture.height = height;
        texture.format = TextureFormat::RGBA;
        texture.wrap = TextureWrap::Repeat;
        texture.filter = TextureFilter::Linear;
        m_ActiveTextures.push_back(i);

        glGenTextures(1, &texture.rendererID);
        glBindTexture(GL_TEXTURE_2D, texture.rendererID);

        // Create texture data
        std::vector<uint32_t> textureData(width * height, color);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        texture.isLoaded = true;
        return i;
      }
    }
    FD_CORE_ERROR("Texture pool is full!");
    return MaxTextures;
  }

  uint32_t TextureSystem::CreateTextureCube(const std::array<const char*, 6>& paths) {
    for (size_t i = 0; i < MaxTextures; ++i) {
      if (!m_Used[i]) {
        m_Used[i] = true;
        TextureData& texture = m_TexturePool[i];
        for (int j = 0; j < 6; ++j) {
          texture.paths[j] = paths[j];
        }
        texture.type = TextureType::TextureCube;
        texture.isLoaded = false;
        texture.wrap = TextureWrap::Clamp;
        texture.filter = TextureFilter::Linear;
        m_ActiveTextures.push_back(i);
        LoadTextureCube(texture);
        return i;
      }
    }
    FD_CORE_ERROR("Texture pool is full!");
    return MaxTextures;
  }

  void TextureSystem::DeleteTexture(uint32_t textureID) {
    if (textureID < MaxTextures && m_Used[textureID]) {
      m_Used[textureID] = false;
      TextureData& texture = m_TexturePool[textureID];
      if (texture.isLoaded) {
        glDeleteTextures(1, &texture.rendererID);
      }
      auto it = std::find(m_ActiveTextures.begin(), m_ActiveTextures.end(), textureID);
      if (it != m_ActiveTextures.end()) {
        m_ActiveTextures.erase(it);
      }
    }
  }

  void TextureSystem::BindTexture(uint32_t textureID, uint32_t slot) const {
    if (textureID < MaxTextures && m_Used[textureID]) {
      const TextureData& texture = m_TexturePool[textureID];
      if (texture.isLoaded) {
        glActiveTexture(GL_TEXTURE0 + slot);
        GLenum target = (texture.type == TextureType::Texture2D) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
        glBindTexture(target, texture.rendererID);
      }
    }
  }

  uint32_t TextureSystem::GetWidth(uint32_t textureID) const {
    return (textureID < MaxTextures && m_Used[textureID]) ? m_TexturePool[textureID].width : 0;
  }

  uint32_t TextureSystem::GetHeight(uint32_t textureID) const {
    return (textureID < MaxTextures && m_Used[textureID]) ? m_TexturePool[textureID].height : 0;
  }

  TextureType TextureSystem::GetTextureType(uint32_t textureID) const {
    return (textureID < MaxTextures && m_Used[textureID]) ? m_TexturePool[textureID].type : TextureType::Texture2D;
  }

  void TextureSystem::LoadTexture2D(TextureData& texture) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(texture.paths[0].c_str(), &width, &height, &channels, 0);

    if (data) {
      texture.width = width;
      texture.height = height;

      GLenum internalFormat = 0, dataFormat = 0;
      if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
        texture.format = TextureFormat::RGBA;
      }
      else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
        texture.format = TextureFormat::RGB;
      }

      glGenTextures(1, &texture.rendererID);
      glBindTexture(GL_TEXTURE_2D, texture.rendererID);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      stbi_image_free(data);
      texture.isLoaded = true;
    }
    else {
      FD_CORE_ERROR("Failed to load texture: {0}", texture.paths[0]);
    }
  }

  void TextureSystem::LoadTextureCube(TextureData& texture) {
    glGenTextures(1, &texture.rendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.rendererID);

    int width, height, channels;
    for (unsigned int i = 0; i < 6; ++i) {
      unsigned char* data = stbi_load(texture.paths[i].c_str(), &width, &height, &channels, 0);
      if (data) {
        GLenum internalFormat = channels == 4 ? GL_RGBA8 : GL_RGB8;
        GLenum dataFormat = channels == 4 ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
      }
      else {
        FD_CORE_ERROR("Failed to load cubemap face: {0}", texture.paths[i]);
        stbi_image_free(data);
        return;
      }
    }

    texture.width = width;
    texture.height = height;
    texture.format = channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    texture.isLoaded = true;
  }



  uint32_t TextureSystem::CreateSubTexture(uint32_t parentTextureID, const glm::vec2& min, const glm::vec2& max) {
    if (parentTextureID >= MaxTextures || !m_Used[parentTextureID]) {
      FD_CORE_ERROR("Invalid parent texture ID!");
      return MaxTextures;
    }

    for (size_t i = 0; i < MaxTextures; ++i) {
      if (!m_Used[i]) {
        m_Used[i] = true;
        TextureData& subTexture = m_TexturePool[i];
        subTexture = m_TexturePool[parentTextureID]; // Copy parent texture data
        subTexture.subTextureMin = min;
        subTexture.subTextureMax = max;
        m_ActiveTextures.push_back(i);
        return i;
      }
    }
    FD_CORE_ERROR("Texture pool is full!");
    return MaxTextures;
  }

  uint32_t TextureSystem::CreateSubTextureFromCoords(uint32_t parentTextureID, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& cellSize) {
    if (parentTextureID >= MaxTextures || !m_Used[parentTextureID]) {
      FD_CORE_ERROR("Invalid parent texture ID!");
      return MaxTextures;
    }

    const TextureData& parentTexture = m_TexturePool[parentTextureID];

    // Calculate UV coordinates with (0,0) at top-left
    glm::vec2 min = {
        (coords.x * cellSize.x) / parentTexture.width,
        (coords.y * cellSize.y) / parentTexture.height
    };
    glm::vec2 max = {
        ((coords.x + spriteSize.x) * cellSize.x) / parentTexture.width,
        ((coords.y + spriteSize.y) * cellSize.y) / parentTexture.height
    };

    return CreateSubTexture(parentTextureID, min, max);
  }

  const glm::vec2* TextureSystem::GetSubTextureCoords(uint32_t textureID) const {
    if (textureID < MaxTextures && m_Used[textureID]) {
      const TextureData& texture = m_TexturePool[textureID];

      
      static glm::vec2 coords[4];
      coords[0] = { texture.subTextureMin.x, texture.subTextureMin.y };
      coords[1] = { texture.subTextureMax.x, texture.subTextureMin.y };
      coords[2] = { texture.subTextureMax.x, texture.subTextureMax.y };
      coords[3] = { texture.subTextureMin.x, texture.subTextureMax.y };

      return coords;
    }
    return nullptr;
  }
}
