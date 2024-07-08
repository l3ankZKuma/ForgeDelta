#include "fdpch.h"
#include "LayerStack.h"

namespace ForgeDelta {

  void ShutdownLayerStack(LayerStack& stack) {
    for (LayerData& layer : stack.Layers) {
      OnLayerDetach(&layer); // Passing the address of layer
    }
    stack.Layers.clear();
  }

  void PushLayer(LayerStack& stack, const LayerData& layer) {
    stack.Layers.insert(stack.Layers.begin() + stack.LayerInsertIndex, layer);
    OnLayerAttach(&stack.Layers[stack.LayerInsertIndex]); // Passing the address of layer
    stack.LayerInsertIndex++;
  }

  void PushOverlay(LayerStack& stack, const LayerData& overlay) {
    stack.Layers.push_back(overlay);
    OnLayerAttach(&stack.Layers.back()); // Passing the address of layer
  }

  void PopLayer(LayerStack& stack, const std::string& debugName) {
    auto it = std::find_if(stack.Layers.begin(), stack.Layers.begin() + stack.LayerInsertIndex,
      [&debugName](const LayerData& layer) { return layer.m_DebugName == debugName; });
    if (it != stack.Layers.begin() + stack.LayerInsertIndex) {
      OnLayerDetach(&(*it)); // Passing the address of layer
      stack.Layers.erase(it);
      stack.LayerInsertIndex--;
    }
  }

  void PopOverlay(LayerStack& stack, const std::string& debugName) {
    auto it = std::find_if(stack.Layers.begin() + stack.LayerInsertIndex, stack.Layers.end(),
      [&debugName](const LayerData& layer) { return layer.m_DebugName == debugName; });
    if (it != stack.Layers.end()) {
      OnLayerDetach(&(*it)); // Passing the address of layer
      stack.Layers.erase(it);
    }
  }

  void UpdateLayers(LayerStack& stack, TimeStep ts) {
    for (LayerData& layer : stack.Layers) {
      OnLayerUpdate(&layer, ts); // Passing the address of layer
    }
  }

  void RenderImGuiLayers(LayerStack& stack) {
    for (LayerData& layer : stack.Layers) {
      OnImGuiRender(&layer); // Passing the address of layer
    }
  }

  void HandleEvent(LayerStack& stack, Event& e) {
    for (LayerData& layer : stack.Layers) {
      OnLayerEvent(&layer, e); // Passing the address of layer
    }
  }

}
