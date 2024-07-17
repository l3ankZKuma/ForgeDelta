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
    constexpr static const char* DefaultTitle = "ForgeDelta";
    constexpr static uint32_t DefaultWidth = 1280;
    constexpr static uint32_t DefaultHeight = 720;
    constexpr static bool DefaultVSync =true;

    const char* Title = DefaultTitle;
    uint32_t Width = DefaultWidth;
    uint32_t Height = DefaultHeight;
    bool VSync = DefaultVSync;
    GLFWwindow* GLFWWindow = nullptr;
    EventCallbackFn EventCallback;
    GraphicsContext* Context = nullptr;
  };

  void InitializeWindow(Window* windowData);
  void ShutdownWindow(Window* windowData);
  void OnWindowUpdate(Window* windowData);
  void SetVSync(Window* windowData, bool enabled);
  void SetEventCallback(Window* windowData, const EventCallbackFn& callback);


}
