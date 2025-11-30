#pragma once

#include <GLFW/glfw3.h>

class Input
{
public:
	static void update(GLFWwindow* window);
	
	static bool isKeyPressed(int keycode);
	static bool isKeyReleased(int keycode);
	static bool isKeyDown(int keycode);
private:
	static bool s_Keys[GLFW_KEY_LAST + 1];
	static bool s_KeysLastFrame[GLFW_KEY_LAST + 1];
};

