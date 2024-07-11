#pragma once

#include "ForgeDelta/Core/Layer.h"
 
namespace ForgeDelta {
  class Event;
  class TimeStep;
} 


namespace ForgeDelta {
  class ImGuiLayer : public Layer
  {

    public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& e) override;
    void OnUpdate(TimeStep ts) override;
    void OnImGuiRender() override; 

    void Begin();
    void End();

    void BlockEvents(bool block) { m_BlockEvents = block; }


  private:
        bool m_BlockEvents = true;


  };
}

