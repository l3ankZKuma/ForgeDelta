#pragma once

#include <functional>
#include<string>

class GLFWwindow;

namespace ForgeDelta {
  class Event;
}

namespace ForgeDelta {

  using EventCallbackFn = std::function<void(Event&)>;

  struct WindowData {
    std::string Title = "ForgeDelta";
    uint32_t Width = 1280;
    uint32_t Height = 720;
    bool VSync = true;
    GLFWwindow* GLFWWindow = nullptr;
    EventCallbackFn EventCallback;
  };

  void InitializeWindow(WindowData* windowData);
  void ShutdownWindow(WindowData* windowData);
  void OnWindowUpdate(WindowData* windowData);
  void SetVSync(WindowData* windowData, bool enabled);
  void SetEventCallback(WindowData* windowData, const EventCallbackFn& callback);


}
