#include "fdpch.h"
#include "Input.h"
#include "ForgeDelta/Core/Application.h"
#include "ForgeDelta/Core/KeyCodes.h"

namespace ForgeDelta {

  bool Input::IsKeyPressed(KeyCode key) {
    auto &window = Application::Get().GetWindow();
    auto state = glfwGetKey(window.GLFWWindow, static_cast<int32_t>(key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool Input::IsMouseButtonPressed(MouseButton button) {
    auto &window = Application::Get().GetWindow();
    auto state = glfwGetMouseButton(window.GLFWWindow, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
  }

  glm::vec2 Input::GetMousePosition() {
    auto &window = Application::Get().GetWindow();
    double xPos, yPos;
    glfwGetCursorPos(window.GLFWWindow, &xPos, &yPos);
    return glm::vec2((float)xPos, (float)yPos);
  }

  float Input::GetMouseX() {
    glm::vec2 pos = GetMousePosition();
    return pos.x;
  }

  float Input::GetMouseY() {
    glm::vec2 pos = GetMousePosition();
    return pos.y;
  }
}
