#include "fdpch.h"
#include "Input.h"
#include "ForgeDelta/Core/Log.h"

namespace ForgeDelta {

  static ApplicationData* s_app = nullptr;
  static GLFWwindow* s_window = nullptr;

  void InjectApplicationToInput(ApplicationData* app) {
    s_app = app;
    s_window = app->m_Window->GLFWWindow;
  }

  //void Input::Update() {
  //  // This method should be called every frame to update the input states
  //  FD_CORE_INFO("Updating input states...");
  //  TransitionPressedKeys();
  //  TransitionPressedButtons();
  //  // Update controller states
  //  for (auto& [id, controller] : s_Controllers) {
  //    for (auto& [button, data] : controller.ButtonStates) {
  //      data.OldState = data.State;
  //      data.State = s_Controllers[id].ButtonDown[button] ? KeyState::Pressed : KeyState::Released;
  //      FD_CORE_INFO("Controller {0} Button {1} State: {2}", id, button, static_cast<int>(data.State));
  //    }
  //  }
  //}

  bool Input::IsKeyPressed(KeyCode keycode) {
    return s_KeyData[keycode].State == KeyState::Pressed;
  }

  bool Input::IsKeyHeld(KeyCode keycode) {
    return s_KeyData[keycode].State == KeyState::Pressed || s_KeyData[keycode].State == KeyState::Held;
  }

  bool Input::IsKeyDown(KeyCode keycode) {
    return s_KeyData[keycode].State == KeyState::Pressed;
  }

  bool Input::IsKeyReleased(KeyCode keycode) {
    return s_KeyData[keycode].State == KeyState::Released;
  }

  bool Input::IsMouseButtonPressed(MouseButton button) {
    return s_MouseData[button].State == KeyState::Pressed;
  }

  bool Input::IsMouseButtonHeld(MouseButton button) {
    return s_MouseData[button].State == KeyState::Pressed || s_MouseData[button].State == KeyState::Held;
  }

  bool Input::IsMouseButtonDown(MouseButton button) {
    return s_MouseData[button].State == KeyState::Pressed;
  }

  bool Input::IsMouseButtonReleased(MouseButton button) {
    return s_MouseData[button].State == KeyState::Released;
  }

  float Input::GetMouseX() {
    double xpos, ypos;
    auto window = s_app->m_Window->GLFWWindow;
    glfwGetCursorPos(window, &xpos, &ypos);
    return (float)(xpos);
  }

  float Input::GetMouseY() {
    double xpos, ypos;
    auto window = s_app->m_Window->GLFWWindow;
    glfwGetCursorPos(window, &xpos, &ypos);
    return (float)(ypos);
  }

  std::pair<float, float> Input::GetMousePosition() {
    return { GetMouseX(), GetMouseY() };
  }

  /*void Input::SetCursorMode(CursorMode mode) {
    FD_CORE_INFO("Setting cursor mode to {0}", static_cast<int>(mode));
    if (mode == CursorMode::Disabled) {
      glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else if (mode == CursorMode::Hidden) {
      glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    else {
      glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

  CursorMode Input::GetCursorMode() {
    int mode = glfwGetInputMode(s_window, GLFW_CURSOR);
    FD_CORE_INFO("Current cursor mode: {0}", mode);
    if (mode == GLFW_CURSOR_DISABLED) {
      return CursorMode::Disabled;
    }
    else if (mode == GLFW_CURSOR_HIDDEN) {
      return CursorMode::Hidden;
    }
    else {
      return CursorMode::Normal;
    }
  }

  bool Input::IsControllerPresent(int id) {
    FD_CORE_INFO("Checking if controller {0} is present", id);
    return s_Controllers.find(id) != s_Controllers.end();
  }

  std::vector<int> Input::GetConnectedControllerIDs() {
    std::vector<int> ids;
    for (const auto& [id, controller] : s_Controllers) {
      ids.push_back(id);
    }
    FD_CORE_INFO("Connected controllers: {0}", ids.size());
    return ids;
  }

  const Controller* Input::GetController(int id) {
    if (IsControllerPresent(id)) {
      FD_CORE_INFO("Getting controller {0}", id);
      return &s_Controllers[id];
    }
    FD_CORE_INFO("Controller {0} not found", id);
    return nullptr;
  }

  std::string_view Input::GetControllerName(int id) {
    if (IsControllerPresent(id)) {
      FD_CORE_INFO("Getting name for controller {0}", id);
      return s_Controllers[id].Name;
    }
    return "";
  }

  bool Input::IsControllerButtonPressed(int controllerID, int button) {
    FD_CORE_INFO("Checking if controller {0} button {1} is pressed", controllerID, button);
    return s_Controllers[controllerID].ButtonStates[button].State == KeyState::Pressed;
  }

  bool Input::IsControllerButtonHeld(int controllerID, int button) {
    FD_CORE_INFO("Checking if controller {0} button {1} is held", controllerID, button);
    return s_Controllers[controllerID].ButtonStates[button].State == KeyState::Pressed || s_Controllers[controllerID].ButtonStates[button].State == KeyState::Held;
  }

  bool Input::IsControllerButtonDown(int controllerID, int button) {
    FD_CORE_INFO("Checking if controller {0} button {1} is down", controllerID, button);
    return s_Controllers[controllerID].ButtonStates[button].State == KeyState::Pressed;
  }

  bool Input::IsControllerButtonReleased(int controllerID, int button) {
    FD_CORE_INFO("Checking if controller {0} button {1} is released", controllerID, button);
    return s_Controllers[controllerID].ButtonStates[button].State == KeyState::Released;
  }

  float Input::GetControllerAxis(int controllerID, int axis) {
    FD_CORE_INFO("Getting axis {0} value for controller {1}", axis, controllerID);
    return s_Controllers[controllerID].AxisStates[axis];
  }

  uint8_t Input::GetControllerHat(int controllerID, int hat) {
    FD_CORE_INFO("Getting hat {0} value for controller {1}", hat, controllerID);
    return s_Controllers[controllerID].HatStates[hat];
  }

  float Input::GetControllerDeadzone(int controllerID, int axis) {
    FD_CORE_INFO("Getting deadzone for controller {0} axis {1}", controllerID, axis);
    return s_Controllers[controllerID].DeadZones[axis];
  }

  void Input::SetControllerDeadzone(int controllerID, int axis, float deadzone) {
    FD_CORE_INFO("Setting deadzone for controller {0} axis {1} to {2}", controllerID, axis, deadzone);
    s_Controllers[controllerID].DeadZones[axis] = deadzone;
  }

  void Input::TransitionPressedKeys() {
    FD_CORE_INFO("Transitioning pressed keys...");
    for (auto& [key, data] : s_KeyData) {
      if (data.State == KeyState::Pressed) {
        data.State = KeyState::Held;
      }
      else if (data.State == KeyState::Released) {
        data.State = KeyState::None;
      }
      data.OldState = data.State;
    }
  }

  void Input::TransitionPressedButtons() {
    FD_CORE_INFO("Transitioning pressed buttons...");
    for (auto& [button, data] : s_MouseData) {
      if (data.State == KeyState::Pressed) {
        data.State = KeyState::Held;
      }
      else if (data.State == KeyState::Released) {
        data.State = KeyState::None;
      }
      data.OldState = data.State;
    }
  }

  void Input::UpdateKeyState(KeyCode key, KeyState newState) {
    FD_CORE_INFO("Updating key {0} state to {1}", static_cast<int>(key), static_cast<int>(newState));
    s_KeyData[key].State = newState;
  }

  void Input::UpdateButtonState(MouseButton button, KeyState newState) {
    FD_CORE_INFO("Updating mouse button {0} state to {1}", static_cast<int>(button), static_cast<int>(newState));
    s_MouseData[button].State = newState;
  }

  void Input::UpdateControllerButtonState(int controller, int button, KeyState newState) {
    FD_CORE_INFO("Updating controller {0} button {1} state to {2}", controller, button, static_cast<int>(newState));
    s_Controllers[controller].ButtonStates[button].State = newState;
  }

  void Input::ClearReleasedKeys() {
    FD_CORE_INFO("Clearing released keys...");
    for (auto& [key, data] : s_KeyData) {
      if (data.State == KeyState::Released) {
        data.State = KeyState::None;
      }
    }
  }*/

}
