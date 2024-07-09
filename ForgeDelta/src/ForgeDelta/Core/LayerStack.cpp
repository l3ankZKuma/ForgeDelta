#include "fdpch.h"
#include "LayerStack.h"
#include "ForgeDelta/Core/Layer.h"


namespace ForgeDelta {

  void ShutdownLayerStack(LayerStack& stack) {
    for (LayerData* layer : stack.Layers) {
      OnLayerDetach(layer);
      delete layer;
    }
    stack.Layers.clear();
  }

  void PushLayer(LayerStack& stack, LayerData* layer) {
    stack.Layers.emplace(stack.Layers.begin() + stack.LayerInsertIndex, layer);
    OnLayerAttach(stack.Layers[stack.LayerInsertIndex]);
    stack.LayerInsertIndex++;
  }

  void PushOverlay(LayerStack& stack, LayerData* overlay) {
    stack.Layers.emplace_back(overlay);
    OnLayerAttach(stack.Layers.back());
  }

  void PopLayer(LayerStack& stack, const std::string& debugName) {
    auto it = std::find_if(stack.Layers.begin(), stack.Layers.begin() + stack.LayerInsertIndex,
      [&debugName](LayerData* layer) { return layer->m_DebugName == debugName; });
    if (it != stack.Layers.begin() + stack.LayerInsertIndex) {
      OnLayerDetach(*it);
      delete* it;
      stack.Layers.erase(it);
      stack.LayerInsertIndex--;
    }
  }

  void PopOverlay(LayerStack& stack, const std::string& debugName) {
    auto it = std::find_if(stack.Layers.begin() + stack.LayerInsertIndex, stack.Layers.end(),
      [&debugName](LayerData* layer) { return layer->m_DebugName == debugName; });
    if (it != stack.Layers.end()) {
      OnLayerDetach(*it);
      delete* it;
      stack.Layers.erase(it);
    }
  }
}
