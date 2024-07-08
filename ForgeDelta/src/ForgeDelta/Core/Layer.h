#pragma once
#include"ForgeDelta/Core/TimeStep.h"
#include"ForgeDelta/Core/Events/Event.h"

namespace ForgeDelta {

  struct LayerData {
    std::string m_DebugName;
  };

  void OnLayerAttach(LayerData* layerData);
  void OnLayerDetach(LayerData* layerData);
  void OnLayerUpdate(LayerData* layerData,TimeStep ts);
  void OnLayerEvent(LayerData* layerData, Event& e);
  void OnImGuiRender(LayerData* layerData);

}