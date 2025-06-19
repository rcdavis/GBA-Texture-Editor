#pragma once

#include "Input/KeyCodes.h"
#include "Input/MouseButtonCodes.h"

#include <glm/glm.hpp>

class Input {
public:
	Input() = delete;
	Input(const Input &) = delete;
	Input &operator=(const Input &) = delete;

	static bool IsKeyPressed(const KeyCode keycode);
	static bool IsKeyReleased(const KeyCode keycode);
	static bool IsMouseButtonPressed(const MouseCode button);

	static glm::vec2 GetMousePosition();
	static f32 GetMouseX();
	static f32 GetMouseY();
};
