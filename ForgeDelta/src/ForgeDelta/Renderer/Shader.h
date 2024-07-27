#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"

namespace ForgeDelta {

  struct ShaderData {
    std::string Name;
    GLuint RendererID;
    std::unordered_map<std::string, GLuint> UniformLocationCache;
  };

  class MemoryPool {
  public:
    MemoryPool(size_t size);
    ~MemoryPool();

    void* allocate(size_t size);
    void deallocate(void* ptr);

  private:
    std::vector<void*> freeList;
    void* pool;
  };

  class ShaderSystem {
  public:
    static void CreateShader(ShaderData& shaderData);
    static void DeleteShader(ShaderData& shaderData);
    static void BindShader(const ShaderData& shaderData);
    inline static void UnbindShader() { glUseProgram(0); }

    static void UploadUniformInt(ShaderData& shaderData, const char* name, int value);
    static void UploadUniformIntArray(ShaderData& shaderData, const char* name, int* values, uint32_t count);
    static void UploadUniformFloat(ShaderData& shaderData, const char* name, float value);
    static void UploadUniformFloat2(ShaderData& shaderData, const char* name, const glm::vec2& value);
    static void UploadUniformFloat3(ShaderData& shaderData, const char* name, const glm::vec3& value);
    static void UploadUniformFloat4(ShaderData& shaderData, const char* name, const glm::vec4& value);
    static void UploadUniformMat4(ShaderData& shaderData, const char* name, const glm::mat4& value);
    static void UploadUniformBool(ShaderData& shaderData, const char* name, bool value);

    static std::string ReadFile(const char* filePath);
    static std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
    static void Compile(GLuint program, const std::unordered_map<GLenum, std::string>& shaderSources);

  private:
    static GLuint GetUniformLocation(ShaderData& shaderData, const char* name);
  };

  class ShaderLibrary {
  public:
    ShaderLibrary();
    ~ShaderLibrary();

    void Add(ShaderData& shaderData);
    ShaderData Load(const char* filePath);
    ShaderData Load(const char* name, const char* filePath);
    ShaderData& Get(const std::string& name);
    inline GLuint GetIndex(const std::string& name) const {
      return m_ShaderIndices.at(name);
    }

  private:
    std::unordered_map<std::string, GLuint> m_ShaderIndices;
    std::vector<ShaderData> m_Shaders;
    MemoryPool m_MemoryPool;
  };

}
