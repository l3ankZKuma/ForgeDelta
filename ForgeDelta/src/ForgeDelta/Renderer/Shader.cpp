#include "fdpch.h"
#include "Shader.h"

#include"ForgeDelta/Core/Log.h"

namespace ForgeDelta {

  MemoryPool::MemoryPool(size_t size) {
    pool = std::malloc(size);
  }

  MemoryPool::~MemoryPool() {
    std::free(pool);
  }

  void* MemoryPool::allocate(size_t size) {
    if (!freeList.empty()) {
      void* ptr = freeList.back();
      freeList.pop_back();
      return ptr;
    }
    return std::malloc(size);
  }

  void MemoryPool::deallocate(void* ptr) {
    freeList.push_back(ptr);
  }

  static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == "vertex") return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
    FD_CORE_ERROR("Unknown shader type!");
    return 0;
  }

  void ShaderSystem::CreateShader(ShaderData& shaderData) {
    shaderData.RendererID = glCreateProgram();
    if (shaderData.RendererID == 0) {
      FD_CORE_ERROR("Error creating shader program!");
    }
  }

  void ShaderSystem::DeleteShader(ShaderData& shaderData) {
    glDeleteProgram(shaderData.RendererID);
  }

  void ShaderSystem::BindShader(const ShaderData& shaderData) {
    glUseProgram(shaderData.RendererID);
  }

  GLuint ShaderSystem::GetUniformLocation(ShaderData& shaderData, const char* name) {
    auto& uniformCache = shaderData.UniformLocationCache;
    if (uniformCache.find(name) != uniformCache.end()) {
      return uniformCache[name];
    }

    GLuint location = glGetUniformLocation(shaderData.RendererID, name);
    uniformCache[name] = location;

    return location;
  }

  void ShaderSystem::UploadUniformInt(ShaderData& shaderData, const char* name, int value) {
    GLuint location = GetUniformLocation(shaderData, name);
    glUniform1i(location, value);
  }

  void ShaderSystem::UploadUniformIntArray(ShaderData& shaderData, const char* name, int* values, uint32_t count) {
    GLuint location = GetUniformLocation(shaderData, name);
    glUniform1iv(location, count, values);
  }

  void ShaderSystem::UploadUniformFloat(ShaderData& shaderData, const char* name, float value) {
    GLuint location = GetUniformLocation(shaderData, name);
    glUniform1f(location, value);
  }

  void ShaderSystem::UploadUniformFloat2(ShaderData& shaderData, const char* name, const glm::vec2& value) {
    GLuint location = GetUniformLocation(shaderData, name);
    glUniform2f(location, value.x, value.y);
  }

  void ShaderSystem::UploadUniformFloat3(ShaderData& shaderData, const char* name, const glm::vec3& value) {
    GLuint location = GetUniformLocation(shaderData, name);
    glUniform3f(location, value.x, value.y, value.z);
  }

  void ShaderSystem::UploadUniformFloat4(ShaderData& shaderData, const char* name, const glm::vec4& value) {
    GLuint location = GetUniformLocation(shaderData, name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
  }

  void ShaderSystem::UploadUniformMat4(ShaderData& shaderData, const char* name, const glm::mat4& value) {
    GLuint location = GetUniformLocation(shaderData, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
  }

  void ShaderSystem::UploadUniformBool(ShaderData& shaderData, const char* name, bool value) {
    GLuint location = GetUniformLocation(shaderData, name);
    glUniform1i(location, value);
  }

  std::string ShaderSystem::ReadFile(const char* filePath) {
    std::string result;
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (file) {
      file.seekg(0, std::ios::end);
      result.resize(file.tellg());
      file.seekg(0, std::ios::beg);
      file.read(&result[0], result.size());
      file.close();
      FD_CORE_INFO("Successfully read shader file: {0}", filePath);
    }
    else {
      FD_CORE_ERROR("Failed to open shader file: {0}", filePath);
    }
    return result;
  }

  std::unordered_map<GLenum, std::string> ShaderSystem::Preprocess(const std::string& source) {
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

  void ShaderSystem::Compile(GLuint program, const std::unordered_map<GLenum, std::string>& shaderSources) {
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

  ShaderLibrary::ShaderLibrary() : m_MemoryPool(1024 * 1024) {}

  ShaderLibrary::~ShaderLibrary() {}

  void ShaderLibrary::Add(ShaderData& shaderData) {
    const std::string& name = shaderData.Name;
    m_ShaderIndices[name] = m_Shaders.size();
    m_Shaders.emplace_back(shaderData);
  }

  ShaderData ShaderLibrary::Load(const char* filePath) {
    ShaderData shaderData;
    ShaderSystem::CreateShader(shaderData);
    std::string source = ShaderSystem::ReadFile(filePath);
    if (source.empty()) {
      FD_CORE_ERROR("Shader source is empty!");
      return shaderData;
    }
    auto shaderSources = ShaderSystem::Preprocess(source);
    GLuint program = glCreateProgram();
    ShaderSystem::Compile(program, shaderSources);
    shaderData.RendererID = program;

    auto lastSlash = std::string(filePath).find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = std::string(filePath).rfind('.');
    auto nameLength = lastDot == std::string::npos ? std::string(filePath).size() - lastSlash : lastDot - lastSlash;
    shaderData.Name = std::string(filePath).substr(lastSlash, nameLength);

    FD_CORE_INFO("Adding shader: {0}", shaderData.Name);
    Add(shaderData);
    return shaderData;
  }

  ShaderData ShaderLibrary::Load(const char* name, const char* filePath) {
    ShaderData shaderData;
    ShaderSystem::CreateShader(shaderData);
    std::string source = ShaderSystem::ReadFile(filePath);
    if (source.empty()) {
      FD_CORE_ERROR("Shader source is empty!");
      return shaderData;
    }
    auto shaderSources = ShaderSystem::Preprocess(source);
    GLuint program = glCreateProgram();
    ShaderSystem::Compile(program, shaderSources);
    shaderData.RendererID = program;
    shaderData.Name = name;

    FD_CORE_INFO("Adding shader: {0}", name);
    Add(shaderData);
    return shaderData;
  }

  ShaderData& ShaderLibrary::Get(const std::string& name) {
    auto it = m_ShaderIndices.find(name);
    if (it != m_ShaderIndices.end()) {
      GLuint index = it->second;
      return m_Shaders[index];
    }
    throw std::runtime_error("Shader not found!");
  }

}
