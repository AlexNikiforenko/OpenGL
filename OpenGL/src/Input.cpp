#include "Input.h"

#include <algorithm>

bool Input::s_Keys[GLFW_KEY_LAST + 1] = { false };
bool Input::s_KeysLastFrame[GLFW_KEY_LAST + 1] = { false };

void Input::update(GLFWwindow* window) {
	if (!window) return;

	std::copy(std::begin(s_Keys), std::end(s_Keys), std::begin(s_KeysLastFrame));

	for (int key = 0; key <= GLFW_KEY_LAST; ++key) {
		s_Keys[key] = (glfwGetKey(window, key) == GLFW_PRESS);
	}
}

bool Input::isKeyPressed(int keycode) {
	if (keycode >= 0 && keycode <= GLFW_KEY_LAST) {
		return s_Keys[keycode] && !s_KeysLastFrame[keycode];
	}
	return false;
}

bool Input::isKeyReleased(int keycode) {
	if (keycode >= 0 && keycode <= GLFW_KEY_LAST) {
		return !s_Keys[keycode] && s_KeysLastFrame[keycode];
	}
	return false;
}

bool Input::isKeyDown(int keycode) {
	if (keycode >= 0 && keycode <= GLFW_KEY_LAST) {
		return s_Keys[keycode];
	}
	return false;
}