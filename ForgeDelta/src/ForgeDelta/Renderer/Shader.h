#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"
#include <unordered_map>

namespace ForgeDelta {

  struct ShaderData {
    std::vector<std::string> Names;
    std::vector<GLuint> RendererIDs;
    std::vector<std::unordered_map<std::string, GLuint>> UniformLocationCaches;
    GLuint Count = 0;
  };

  class OpenGLShaderService {
  public:
    static void CreateShader(ShaderData& shaderData, GLuint count);
    static void DeleteShader(ShaderData& shaderData);
    static void BindShader(const ShaderData& shaderData, GLuint index);
    inline static void UnbindShader(){ glUseProgram(0);}

    static void UploadUniformInt(ShaderData& shaderData, GLuint index, const char* name, int value);
    static void UploadUniformIntArray(ShaderData& shaderData, GLuint index, const char* name, int* values, uint32_t count);
    static void UploadUniformFloat(ShaderData& shaderData, GLuint index, const char* name, float value);
    static void UploadUniformFloat2(ShaderData& shaderData, GLuint index, const char* name, const glm::vec2& value);
    static void UploadUniformFloat3(ShaderData& shaderData, GLuint index, const char* name, const glm::vec3& value);
    static void UploadUniformFloat4(ShaderData& shaderData, GLuint index, const char* name, const glm::vec4& value);
    static void UploadUniformMat4(ShaderData& shaderData, GLuint index, const char* name, const glm::mat4& value);

    static std::string ReadFile(const char* filePath);
    static std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
    static void Compile(GLuint program, const std::unordered_map<GLenum, std::string>& shaderSources);

  private:
    static GLuint GetUniformLocation(ShaderData& shaderData, GLuint index, const char* name);
  };

  class ShaderLibrary {
  public:
    ShaderLibrary() = default;
    ~ShaderLibrary() = default;

    void Add(ShaderData& shaderData);
    ShaderData Load(const char* filePath);
    ShaderData Load(const char* name, const char* filePath);
    const ShaderData& Get(const std::string& name) const;
    inline GLuint GetIndex(const std::string& name) const {
      return m_ShaderIndices.at(name); 
    }

  private:
    std::unordered_map<std::string, GLuint> m_ShaderIndices;
    std::vector<ShaderData> m_Shaders;
    GLuint m_ShaderCount = 0;
  };

} // namespace ForgeDelta
