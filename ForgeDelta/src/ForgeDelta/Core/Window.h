#pragma once

#include <functional>
#include<string>

struct GLFWwindow;

namespace ForgeDelta {
  class Event;
  class GraphicsContext;
}

namespace ForgeDelta {

  using EventCallbackFn = std::function<void(Event&)>;

  struct Window {
    const char * Title = "ForgeDelta";
    uint32_t Width = 1280;
    uint32_t Height = 720;
    bool VSync = true;
    GLFWwindow* GLFWWindow = nullptr;
    EventCallbackFn EventCallback;
    GraphicsContext* Context;
    
  };

  void InitializeWindow(Window* windowData);
  void ShutdownWindow(Window* windowData);
  void OnWindowClear(Window* windowData);
  void OnWindowUpdate(Window* windowData);
  void SetVSync(Window* windowData, bool enabled);
  void SetEventCallback(Window* windowData, const EventCallbackFn& callback);


}
