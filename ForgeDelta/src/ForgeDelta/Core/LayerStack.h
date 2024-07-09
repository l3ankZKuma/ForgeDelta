#pragma once

namespace ForgeDelta {
    struct LayerData;

}

namespace ForgeDelta {

  struct LayerStack {
    std::vector<LayerData*> Layers;
    unsigned int LayerInsertIndex = 0;
  };

  void ShutdownLayerStack(LayerStack& stack);
  void PushLayer(LayerStack& stack, LayerData* layer);
  void PushOverlay(LayerStack& stack, LayerData* overlay);
  void PopLayer(LayerStack& stack, const std::string& debugName);
  void PopOverlay(LayerStack& stack, const std::string& debugName);
}
