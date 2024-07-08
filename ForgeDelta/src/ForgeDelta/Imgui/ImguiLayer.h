#pragma once

#include "ForgeDelta/Core/Events/Event.h"

namespace ForgeDelta {

  struct ImGuiLayerData {
    std::string m_DebugName;
    bool m_BlockEvents = true;
  };

  void InitializeImGuiLayer(ImGuiLayerData* layerData);
  void ShutdownImGuiLayer(ImGuiLayerData* layerData);
  void BeginImGuiLayer(ImGuiLayerData* layerData);
  void EndImGuiLayer(ImGuiLayerData* layerData);
  void OnImGuiLayerEvent(ImGuiLayerData* layerData, Event& e);

}