#include "fdpch.h"
#include "Window.h"
#include"ForgeDelta/Core/Events/Event.h"
#include"ForgeDelta/Core/KeyCodes.h"
#include"ForgeDelta/Core/Events/MouseEvent.h"
#include"ForgeDelta/Core/Events/ApplicationEvent.h"
#include"ForgeDelta/Core/Events/KeyEvent.h"


namespace ForgeDelta {

  static bool s_GLFWInitialized = false;

  static void GLFWErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
  }

  void InitializeWindow(WindowData* window) {
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

    window->GLFWWindow = glfwCreateWindow(window->Width, window->Height, window->Title.c_str(), nullptr, nullptr);
    if (!window->GLFWWindow) {
      std::cerr << "Failed to create GLFW window!" << std::endl;
      return;
    }

    glfwMakeContextCurrent(window->GLFWWindow);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD!" << std::endl;
      return;
    }


    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowUserPointer(window->GLFWWindow, window);
    SetVSync(window, window->VSync);




    glfwSetWindowSizeCallback(window->GLFWWindow, [](GLFWwindow* window, int width, int height) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.Width = width;
      data.Height = height;

      WindowResizeEvent event(width, height);
      data.EventCallback(event);
      });


    glfwSetWindowCloseCallback(window->GLFWWindow, [](GLFWwindow* window) {


      WindowCloseEvent event;
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.EventCallback(event);

      });

    glfwSetKeyCallback(window->GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

      switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(key, 1);
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.EventCallback(event);
        break;
      }
      }
      });

    glfwSetCharCallback(window->GLFWWindow, [](GLFWwindow* window, unsigned int keycode) {

      KeyTypedEvent event(keycode);
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.EventCallback(event);

      });


    glfwSetMouseButtonCallback(window->GLFWWindow, [](GLFWwindow* window, int button, int action, int mods) {

      switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.EventCallback(event);
        break;
      }
      }
      });

    glfwSetScrollCallback(window->GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset) {

      MouseScrolledEvent event((float)xOffset, (float)yOffset);
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.EventCallback(event);

      });

    glfwSetCursorPosCallback(window->GLFWWindow, [](GLFWwindow* window, double xPos, double yPos) {

      MouseMovedEvent event((float)xPos, (float)yPos);
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.EventCallback(event);

      });



 

  }

  void ShutdownWindow(WindowData* window) {
    glfwDestroyWindow(window->GLFWWindow);
    glfwTerminate();
  }

  void OnWindowUpdate(WindowData* window) {
    glfwPollEvents();
    glfwSwapBuffers(window->GLFWWindow);
  }

  void SetVSync(WindowData* window, bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    window->VSync = enabled;
  }

  void SetEventCallback(WindowData* window, const EventCallbackFn& callback) {
    window->EventCallback = callback;
  }

}
