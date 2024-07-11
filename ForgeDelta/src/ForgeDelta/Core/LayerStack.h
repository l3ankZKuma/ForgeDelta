#pragma once

namespace ForgeDelta {
  class Layer;

}

namespace ForgeDelta {

  using LayerVector = std::vector<Layer*>;

  class LayerStack {
  public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    LayerVector::iterator begin() { return m_Layers.begin(); }
    LayerVector::iterator end() { return m_Layers.end(); }
    LayerVector::reverse_iterator rbegin() { return m_Layers.rbegin(); }
    LayerVector::reverse_iterator rend() { return m_Layers.rend(); }

    LayerVector::const_iterator begin() const { return m_Layers.cbegin(); }
    LayerVector::const_iterator end() const { return m_Layers.cend(); }
    LayerVector::const_reverse_iterator rbegin() const { return m_Layers.crbegin(); }
    LayerVector::const_reverse_iterator rend() const { return m_Layers.crend(); }

  private:
    LayerVector m_Layers;
    unsigned int m_LayerInsertIndex = 0;
  };

}