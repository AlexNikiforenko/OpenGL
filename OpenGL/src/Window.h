#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Scene.h"
#include "ImGuiLayer.h"


#include <vector>
#include <memory>


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
	void handleResize(int width, int height);
	void handleMouseMovement(double xposIn, double yposIn);
	void handleMouseScroll(double xoffset, double yoffset);

	GLFWwindow* m_Window = nullptr;

	Camera m_Camera;
	Shader m_Shader;
	Shader m_LightShader;

	Scene m_Scene;
	ImGuiLayer m_ImGuiLayer;

	unsigned int m_VBO = 0;
	unsigned int m_CubeVAO = 0;
	unsigned int m_LightVAO = 0;
	unsigned int m_EBO = 0;

	std::unique_ptr<Texture> m_Texture1;
	std::unique_ptr<Texture> m_Texture2;

	int m_Width = 800;
	int m_Height = 600;

	double m_LastX = 0.0;
	double m_LastY = 0.0;
	bool m_FirstMouse = true;

	float m_DeltaTime = 0.0f;	
	float m_LastFrame = 0.0f;
};