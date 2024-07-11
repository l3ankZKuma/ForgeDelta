#include "fdpch.h"
#include "Window.h"

#include"ForgeDelta/Core/Events/Event.h"
#include"ForgeDelta/Core/KeyCodes.h"
#include"ForgeDelta/Core/Events/MouseEvent.h"
#include"ForgeDelta/Core/Events/ApplicationEvent.h"
#include"ForgeDelta/Core/Events/KeyEvent.h"
#include "ForgeDelta/Core/Log.h"

namespace ForgeDelta {

  static bool s_GLFWInitialized = false;

  static void GLFWErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
  }

  void InitializeWindow(Window* window) {
    if (!s_GLFWInitialized) {
      int success = glfwInit();
      if (!success) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return;
      }
      glfwSetErrorCallback(GLFWErrorCallback);
      s_GLFWInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->GLFWWindow = glfwCreateWindow(window->Width, window->Height, window->Title, nullptr, nullptr);
    if (!window->GLFWWindow) {
      std::cerr << "Failed to create GLFW window!" << std::endl;
      return;
    }

    glfwMakeContextCurrent(window->GLFWWindow);
    FD_CORE_INFO("Created Window");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD!" << std::endl;
      return;
    }

    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowUserPointer(window->GLFWWindow, window);
    SetVSync(window, window->VSync);

    glfwSetWindowSizeCallback(window->GLFWWindow, [](GLFWwindow* window, int width, int height) {
      Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
      data.Width = width;
      data.Height = height;

      WindowResizeEvent event(width, height);
      data.EventCallback(event);
      });

    glfwSetWindowCloseCallback(window->GLFWWindow, [](GLFWwindow* window) {
      WindowCloseEvent event;
      Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
      data.EventCallback(event);
      });

    glfwSetKeyCallback(window->GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(key, 1);
        Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        data.EventCallback(event);
        break;
      }
      }
      });

    glfwSetCharCallback(window->GLFWWindow, [](GLFWwindow* window, unsigned int keycode) {
      KeyTypedEvent event(keycode);
      Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
      data.EventCallback(event);
      });

    glfwSetMouseButtonCallback(window->GLFWWindow, [](GLFWwindow* window, int button, int action, int mods) {
      switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        data.EventCallback(event);
        break;
      }
      }
      });

    glfwSetScrollCallback(window->GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
      MouseScrolledEvent event((float)xOffset, (float)yOffset);
      Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
      data.EventCallback(event);
      });

    glfwSetCursorPosCallback(window->GLFWWindow, [](GLFWwindow* window, double xPos, double yPos) {
      MouseMovedEvent event((float)xPos, (float)yPos);
      Window& data = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
      data.EventCallback(event);
      });
  }

  void ShutdownWindow(Window* window) {
    glfwDestroyWindow(window->GLFWWindow);
    glfwTerminate();
  }

  void OnWindowUpdate(Window* window) {
    glfwPollEvents();
    glfwSwapBuffers(window->GLFWWindow);
  }

  void OnWindowClear(Window* window) {
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void SetVSync(Window* window, bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    window->VSync = enabled;
  }

  void SetEventCallback(Window* window, const EventCallbackFn& callback) {
    window->EventCallback = callback;
  }

}
