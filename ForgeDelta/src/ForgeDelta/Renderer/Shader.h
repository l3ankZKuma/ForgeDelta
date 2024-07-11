#pragma once

namespace ForgeDelta {

  struct ShaderData {
    std::vector<uint32_t> m_RendererIDs;
    std::vector<std::string> m_FilePaths;
    std::vector<std::string> m_Names;
  };


  void BindShader(ShaderData & shader);
  void UnbindShader(ShaderData & shader);
}

