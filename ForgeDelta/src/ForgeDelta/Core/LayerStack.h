#pragma once
#include "ForgeDelta/Core/Layer.h"

namespace ForgeDelta {

  struct LayerStack {
    std::vector<LayerData> Layers;
    unsigned int LayerInsertIndex = 0;
  };

  void ShutdownLayerStack(LayerStack& stack);
  void PushLayer(LayerStack& stack, const LayerData& layer);
  void PushOverlay(LayerStack& stack, const LayerData& overlay);
  void PopLayer(LayerStack& stack, const std::string& debugName);
  void PopOverlay(LayerStack& stack, const std::string& debugName);

  void UpdateLayers(LayerStack& stack, TimeStep ts);
  void RenderImGuiLayers(LayerStack& stack);
  void HandleEvent(LayerStack& stack, Event& e);

}
