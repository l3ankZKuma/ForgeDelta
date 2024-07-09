#pragma once


namespace ForgeDelta {
  class TimeStep;
  class Event;
  struct ApplicationData;
}

namespace ForgeDelta {

  enum class LayerType {
    BaseLayer,
    ImGuiLayer
  };

  struct LayerData {
    std::string m_DebugName;
    LayerType m_Type;
    bool m_BlockEvents = true; // For ImGuiLayer
  };

  void OnLayerAttach(LayerData* layerData);
  void OnLayerDetach(LayerData* layerData);
  void OnLayerUpdate(LayerData* layerData, TimeStep ts);
  void OnLayerEvent(LayerData* layerData, Event& e);
  void OnImGuiRender(LayerData* layerData);

  void InjectionApplicationToLayer(ApplicationData* app);

  //for ImGuiLayer
  void BeginImGuiLayer();
  void EndImGuiLayer();
}