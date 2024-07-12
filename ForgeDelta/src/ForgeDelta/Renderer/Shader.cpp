#include "fdpch.h"
#include "Shader.h"
#include "ForgeDelta/Core/Log.h"
#include "ForgeDelta/Core/Base.h"
#include <iostream>
#include <fstream>
#include <vector>

namespace ForgeDelta {

  static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == "vertex") return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
    FD_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
  }

  void OpenGLShaderService::CreateShader(ShaderData& shaderData, GLuint count) {
    shaderData.Count = count;
    shaderData.Names.resize(count);
    shaderData.RendererIDs.resize(count);
    shaderData.UniformLocationCaches.resize(count);

    for (GLuint i = 0; i < count; ++i) {
      shaderData.RendererIDs[i] = glCreateProgram();  // Initialize each program ID
      if (shaderData.RendererIDs[i] == 0) {
        std::cerr << "Error creating shader program!" << std::endl;
      }
    }
  }

  void OpenGLShaderService::DeleteShader(ShaderData& shaderData) {
    for (GLuint i = 0; i < shaderData.Count; ++i) {
      glDeleteProgram(shaderData.RendererIDs[i]);
    }
  }

  void OpenGLShaderService::BindShader(const ShaderData& shaderData, GLuint index) {
    if (index >= shaderData.RendererIDs.size()) {
      std::cerr << "BindShader error: index " << index << " out of range. RendererIDs size: " << shaderData.RendererIDs.size() << std::endl;
      return;
    }
    glUseProgram(shaderData.RendererIDs[index]);
  }

  GLuint OpenGLShaderService::GetUniformLocation(ShaderData& shaderData, GLuint index, const char* name) {
    if (index >= shaderData.UniformLocationCaches.size()) {
      std::cerr << "GetUniformLocation error: index " << index << " out of range. UniformLocationCaches size: " << shaderData.UniformLocationCaches.size() << std::endl;
      return -1;
    }
    auto& uniformCache = shaderData.UniformLocationCaches[index];
    if (uniformCache.find(name) != uniformCache.end()) {
      return uniformCache[name];
    }

    if (index >= shaderData.RendererIDs.size()) {
      std::cerr << "GetUniformLocation error: index " << index << " out of range. RendererIDs size: " << shaderData.RendererIDs.size() << std::endl;
      return -1;
    }

    GLuint location = glGetUniformLocation(shaderData.RendererIDs[index], name);
    uniformCache[name] = location;

    return location;
  }

  void OpenGLShaderService::UploadUniformInt(ShaderData& shaderData, GLuint index, const char* name, int value) {
    GLuint location = GetUniformLocation(shaderData, index, name);
    glUniform1i(location, value);
  }

  void OpenGLShaderService::UploadUniformIntArray(ShaderData& shaderData, GLuint index, const char* name, int* values, uint32_t count) {
    GLuint location = GetUniformLocation(shaderData, index, name);
    glUniform1iv(location, count, values);
  }

  void OpenGLShaderService::UploadUniformFloat(ShaderData& shaderData, GLuint index, const char* name, float value) {
    GLuint location = GetUniformLocation(shaderData, index, name);
    glUniform1f(location, value);
  }

  void OpenGLShaderService::UploadUniformFloat2(ShaderData& shaderData, GLuint index, const char* name, const glm::vec2& value) {
    GLuint location = GetUniformLocation(shaderData, index, name);
    glUniform2f(location, value.x, value.y);
  }

  void OpenGLShaderService::UploadUniformFloat3(ShaderData& shaderData, GLuint index, const char* name, const glm::vec3& value) {
    GLuint location = GetUniformLocation(shaderData, index, name);
    glUniform3f(location, value.x, value.y, value.z);
  }

  void OpenGLShaderService::UploadUniformFloat4(ShaderData& shaderData, GLuint index, const char* name, const glm::vec4& value) {
    GLuint location = GetUniformLocation(shaderData, index, name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
  }

  void OpenGLShaderService::UploadUniformMat4(ShaderData& shaderData, GLuint index, const char* name, const glm::mat4& value) {
    GLuint location = GetUniformLocation(shaderData, index, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
  }

  std::string OpenGLShaderService::ReadFile(const char* filePath) {
    std::string result;
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (file) {
      file.seekg(0, std::ios::end);
      result.resize(file.tellg());
      file.seekg(0, std::ios::beg);
      file.read(&result[0], result.size());
      file.close();
      std::cout << "Successfully read shader file: " << filePath << std::endl;
    }
    else {
      std::cerr << "Failed to open shader file: " << filePath << std::endl;
    }
    return result;
  }

  std::unordered_map<GLenum, std::string> OpenGLShaderService::Preprocess(const std::string& source) {
    std::unordered_map<GLenum, std::string> shaderSources;
    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);

    while (pos != std::string::npos) {
      size_t eol = source.find_first_of("\r\n", pos);
      size_t begin = pos + typeTokenLength + 1;
      std::string type = source.substr(begin, eol - begin);

      size_t nextLinePos = source.find_first_not_of("\r\n", eol);
      pos = source.find(typeToken, nextLinePos);

      shaderSources[ShaderTypeFromString(type)] = pos == std::string::npos ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
  }

  void OpenGLShaderService::Compile(GLuint program, const std::unordered_map<GLenum, std::string>& shaderSources) {
    std::array<GLuint, 2> shaderIDs;
    size_t shaderCount = 0;

    for (const auto& kv : shaderSources) {
      GLenum type = kv.first;
      const std::string& source = kv.second;

      GLuint shader = glCreateShader(type);
      const GLchar* sourceCStr = source.c_str();
      glShaderSource(shader, 1, &sourceCStr, nullptr);
      glCompileShader(shader);

      GLint isCompiled = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
      if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

        glDeleteShader(shader);
        FD_CORE_ERROR("Shader compilation failed: {0}", infoLog.data());
        std::cerr << "Shader compilation failed: " << infoLog.data() << std::endl;
        return;
      }

      glAttachShader(program, shader);
      shaderIDs[shaderCount++] = shader;
    }

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

      glDeleteProgram(program);
      FD_CORE_ERROR("Program linking failed: {0}", infoLog.data());
      std::cerr << "Program linking failed: " << infoLog.data() << std::endl;

      for (size_t i = 0; i < shaderCount; ++i) {
        glDeleteShader(shaderIDs[i]);
      }

      return;
    }

    for (size_t i = 0; i < shaderCount; ++i) {
      glDetachShader(program, shaderIDs[i]);
      glDeleteShader(shaderIDs[i]);
    }
  }

  void ShaderLibrary::Add(ShaderData& shaderData) {
    for (GLuint i = 0; i < shaderData.Count; ++i) {
      const std::string& name = shaderData.Names[i];
      m_ShaderIndices[name] = m_Shaders.size();
      m_Shaders.push_back(shaderData);
    }
  }

  ShaderData ShaderLibrary::Load(const char* filePath) {
    ShaderData shaderData;
    GLuint count = ++m_ShaderCount; // Increment the shader count
    OpenGLShaderService::CreateShader(shaderData, count); // Use the new count value
    std::string source = OpenGLShaderService::ReadFile(filePath);
    if (source.empty()) {
      FD_CORE_ERROR("Shader source is empty!");
      return shaderData;
    }
    auto shaderSources = OpenGLShaderService::Preprocess(source);
    GLuint program = glCreateProgram();
    OpenGLShaderService::Compile(program, shaderSources);
    shaderData.RendererIDs[count - 1] = program;

    auto lastSlash = std::string(filePath).find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = std::string(filePath).rfind('.');
    auto nameLength = lastDot == std::string::npos ? std::string(filePath).size() - lastSlash : lastDot - lastSlash;
    shaderData.Names[count - 1] = std::string(filePath).substr(lastSlash, nameLength);

    std::cout << "Adding shader: " << shaderData.Names[count - 1] << std::endl;
    Add(shaderData);
    return shaderData;
  }

  ShaderData ShaderLibrary::Load(const char* name, const char* filePath) {
    ShaderData shaderData;
    GLuint count = ++m_ShaderCount; // Increment the shader count
    OpenGLShaderService::CreateShader(shaderData, count); // Use the new count value
    std::string source = OpenGLShaderService::ReadFile(filePath);
    if (source.empty()) {
      FD_CORE_ERROR("Shader source is empty!");
      return shaderData;
    }
    auto shaderSources = OpenGLShaderService::Preprocess(source);
    GLuint program = glCreateProgram();
    OpenGLShaderService::Compile(program, shaderSources);
    shaderData.RendererIDs[count - 1] = program;
    shaderData.Names[count - 1] = name;

    std::cout << "Adding shader: " << name << std::endl;
    Add(shaderData);
    return shaderData;
  }

  const ShaderData& ShaderLibrary::Get(const std::string& name) const {
    auto it = m_ShaderIndices.find(name);
    if (it != m_ShaderIndices.end()) {
      GLuint index = it->second;
      return m_Shaders[index];
    }
    // Handle case where shader is not found.
    throw std::runtime_error("Shader not found!");
  }

} // namespace ForgeDelta
