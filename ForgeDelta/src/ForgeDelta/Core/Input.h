#pragma once

#include "ForgeDelta/Core/KeyCodes.h"
namespace ForgeDelta {

	class  Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseButton button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		// Additional functions for 3D input handling
		static glm::vec3 GetMousePosition3D();
		static float GetMouseZ();


	};
}