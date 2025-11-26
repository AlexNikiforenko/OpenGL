#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

#include <vector>

const float SPAWN_DISTANCE = 5.0f;

class Window {
public:

	Window(int width, int height, const char* title);
	~Window();

	void run();
	void update();
	void render();
	void processInput();

	static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void glfwMouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	GLFWwindow* m_Window = nullptr;

	Camera m_Camera;
	Shader m_Shader;

	std::vector<glm::vec3> m_CubePositions;

	unsigned int m_VBO = 0;
	unsigned int m_VAO = 0;
	unsigned int m_EBO = 0;

	unsigned int m_Texture1 = 0;
	unsigned int m_Texture2 = 0;

	int m_Width = 800;
	int m_Height = 600;

	double m_LastX = 0.0;
	double m_LastY = 0.0;
	bool m_FirstMouse = true;
	bool m_PaintingMode = false;

	float m_MixValue = 0.2f;
	float m_RotateSpeed = 0.1f;

	float m_DeltaTime = 0.0f;	// Time between current frame and last frame
	float m_LastFrame = 0.0f; // Time of last frame

	void handleResize(int width, int height);
	void handleMouseMovement(double xposIn, double yposIn);
	void handleMouseScroll(double xoffset, double yoffset);

	static constexpr float SHIFT_BOOST = 3.0f;
	static constexpr float ROTATION_SPEED_RATE = 5.0f;
	static constexpr float MIN_ROTATION_SPEED = 0.0f;
	static constexpr float MAX_ROTATION_SPEED = 30.0f;
	static constexpr float MIX_VALUE_CHANGE_RATE = 1.0f;
	static constexpr float MIN_MIX_VALUE = 0.0f;
	static constexpr float MAX_MIX_VALUE = 1.0f;
};