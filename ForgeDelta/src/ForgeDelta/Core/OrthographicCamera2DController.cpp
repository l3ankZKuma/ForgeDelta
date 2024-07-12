#include "fdpch.h"
#include "OrthographicCamera2DController.h"

#include "ForgeDelta/Core/Input.h"
#include "ForgeDelta/Core/KeyCodes.h"

#include"ForgeDelta/Core/TimeStep.h"
#include"ForgeDelta/Core/Events/Event.h"
#include"ForgeDelta/Core/Events/MouseEvent.h"
#include"ForgeDelta/Core/Events/ApplicationEvent.h"

namespace ForgeDelta {

  OrthographicCamera2DController::OrthographicCamera2DController(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio), m_Rotation(rotation), m_Camera(-aspectRatio* m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {}

  void OrthographicCamera2DController::OnUpdate(TimeStep ts) {

    float moveX = (Input::IsKeyPressed(Key::D)-Input::IsKeyPressed(Key::A) )* m_CameraTranslationSpeed * ts;
    float moveY = (Input::IsKeyPressed(Key::W) - Input::IsKeyPressed(Key::S)) * m_CameraTranslationSpeed * ts;

    m_CameraPosition.x += moveX;
    m_CameraPosition.y += moveY;

    if (m_Rotation) {
      float rotate = (Input::IsKeyPressed(Key::Q) - Input::IsKeyPressed(Key::E)) * m_CameraRotationSpeed * ts;
      m_CameraRotation += rotate;
      m_Camera.SetRotation(m_CameraRotation);
    }

    // Update the camera position
    m_Camera.SetPosition(m_CameraPosition);
  }


  void OrthographicCamera2DController::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);

    auto onMouseScrolled = [this](MouseScrolledEvent& e) -> bool {
      m_ZoomLevel -= e.GetYOffset() * 0.25f;
      m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
      m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_ZoomLevel*m_AspectRatio, -m_ZoomLevel, m_ZoomLevel);
      return false;
      };

    auto onWindowResized = [this](WindowResizeEvent& e) -> bool {
      m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
      m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_ZoomLevel*m_AspectRatio, -m_ZoomLevel, m_ZoomLevel);
      return false;
      };

    dispatcher.Dispatch<MouseScrolledEvent>(onMouseScrolled);
    dispatcher.Dispatch<WindowResizeEvent>(onWindowResized);
  }

} // namespace ForgeCraft
