#pragma once

#include"ForgeDelta/Renderer/Camera/Orthographic2DCamera.h"

namespace ForgeDelta {
  class TimeStep;
  class Event;
}


namespace ForgeDelta {

  class OrthographicCamera2DController {
  public:
    OrthographicCamera2DController(float aspectRatio, bool rotation = false);

    void OnUpdate(TimeStep ts);
    void OnEvent(Event& e);

    [[nodiscard]] inline Orthographic2DCamera& GetCamera() { return  m_Camera; }


  private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    bool m_Rotation;


    Orthographic2DCamera m_Camera;

    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraRotation = 0.0f; // In degrees, in the anti-clockwise direction
    float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;

  };

} // namespace ForgeCraft
