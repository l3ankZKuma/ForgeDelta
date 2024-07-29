#pragma once

#include<bitset>


namespace ForgeDelta {

  enum class TextureType
  {
    Texture2D,
    TextureCube
  };

  enum class TextureFormat
  {
    None = 0,
    RGB = 1,
    RGBA = 2,
    Float16 = 3
  };

  enum class TextureWrap
  {
    None = 0,
    Clamp = 1,
    Repeat = 2
  };

  enum class TextureFilter
  {
    None = 0,
    Nearest = 1,
    Linear = 2
  };

  struct TextureData
  {
    uint32_t rendererID;
    uint32_t width;
    uint32_t height;
    TextureType type;
    TextureFormat format;
    TextureWrap wrap;
    TextureFilter filter;
    std::array<std::string, 6> paths;  // For cubemaps, we need 6 paths
    bool isLoaded;


    glm::vec2 subTextureMin;  // New: for subtexture support
    glm::vec2 subTextureMax;  // New: for subtexture support


  };

  class TextureSystem
  {
  public:
    static constexpr size_t MaxTextures = 1000;

    TextureSystem();
    ~TextureSystem();

    uint32_t CreateTexture2D(const char* path);
    uint32_t CreateTexture2D(uint32_t width, uint32_t height,uint32_t color);
    uint32_t CreateTextureCube(const std::array<const char*, 6>& paths);
    void DeleteTexture(uint32_t textureID);
    void BindTexture(uint32_t textureID, uint32_t slot = 0) const;
    uint32_t GetWidth(uint32_t textureID) const;
    uint32_t GetHeight(uint32_t textureID) const;
    TextureType GetTextureType(uint32_t textureID) const;


    // New SubTexture methods
    uint32_t CreateSubTexture(uint32_t parentTextureID, const glm::vec2& min, const glm::vec2& max);
    uint32_t CreateSubTextureFromCoords(uint32_t parentTextureID, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& cellSize = { 1, 1 });
    const glm::vec2* GetSubTextureCoords(uint32_t textureID) const;

  private:
    std::array<TextureData, MaxTextures> m_TexturePool;
    std::bitset<MaxTextures> m_Used;
    std::vector<uint32_t> m_ActiveTextures;

    void LoadTexture2D(TextureData& texture);
    void LoadTextureCube(TextureData& texture);
  };

  // Global texture system instance
  extern TextureSystem g_TextureSystem;

} // namespace ForgeCraft