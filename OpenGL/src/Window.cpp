#include "Window.h"

#include "CubeVertices.h"
#include "Input.h"

#include "stb_image.h"
#include <iostream>


/*
	// Remove paint tress
	// Choose texture
	// Texture class
	// Collisions
*/

Window::Window(int width, int height, const char* title) 
	: m_Width(width), m_Height(height) {
	// glfw init and config
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_Window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}

	glfwSetWindowUserPointer(m_Window, this);
	glfwMakeContextCurrent(m_Window);

	glfwSetFramebufferSizeCallback(m_Window, Window::glfwFramebufferSizeCallback);
	glfwSetCursorPosCallback(m_Window, Window::glfwMouseCallback);
	glfwSetScrollCallback(m_Window, Window::glfwScrollCallback);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// glad load
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return;
	}

	m_Camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	m_Shader = Shader("shaders/shader.vert", "shaders/shader.frag");

	m_CubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
		glm::vec3(-2.3f,  4.0f, -1.5f),
		glm::vec3(-3.3f,  -3.0f, -1.5f),
		glm::vec3(-4.3f,  1.0f, -1.5f)
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Load and create texture1
	glGenTextures(1, &m_Texture1);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texWidth, texHeight, nrChannels;
	unsigned char* data = stbi_load("resources/textures/image.jpg", &texWidth, &texHeight, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);

	// texture2
	glGenTextures(1, &m_Texture2);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("resources/textures/screamer.jpg", &texWidth, &texHeight, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);

	m_Shader.use();
	m_Shader.setInt("texture1", 0);
	m_Shader.setInt("texture2", 1);

	glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
	glfwTerminate();
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Window::run() {
	while (!glfwWindowShouldClose(m_Window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;

		processInput();
		update();
		render();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void Window::update() {

}

void Window::render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Shader.use();
	float offset = 0.5f;
	float speed = 0.75f;
	float timeValue = static_cast<float>(glfwGetTime());
	m_Shader.setFloat("xOffset", cos(speed * timeValue) * offset);
	m_Shader.setFloat("yOffset", sin(25 * speed * timeValue) * offset);
	m_Shader.setFloat("colorOffset", cos(timeValue / 2.0f));
	m_Shader.setFloat("mixValue", m_MixValue);
	m_Shader.setFloat("time", timeValue);

	glm::mat4 projection = glm::mat4(2.0f);
	projection = glm::perspective(glm::radians(m_Camera.getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);

	m_Shader.setMatrix("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);

	const float radius = 5.0f;
	float camX = static_cast<float>(sin(glfwGetTime()) * radius);
	float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
	glm::mat4 view = m_Camera.getViewMatrix();
	m_Shader.setMatrix("view", view);


	glBindVertexArray(m_VAO);
	for (size_t i = 0; i < m_CubePositions.size(); i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_CubePositions[i]);
		float angle = 20.0f * i + 10.0f;
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle) * m_RotateSpeed, glm::vec3(1.0f, 0.3f, 0.5f));
		m_Shader.setMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Window::handleResize(int width, int height) {
	m_Width = width;
	m_Height = height;

	glViewport(0, 0, width, height);
}

void Window::processInput() {
	Input::update(m_Window);

	float speedMultiplier = 1.0f;

	// Speed boost when left shift is held
	if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		speedMultiplier = SHIFT_BOOST;
	}

	// Rotation speed control
	if (Input::isKeyDown(GLFW_KEY_RIGHT)) {
		m_RotateSpeed += ROTATION_SPEED_RATE * m_DeltaTime;
	}
	if (Input::isKeyDown(GLFW_KEY_LEFT)) {
		m_RotateSpeed -= ROTATION_SPEED_RATE * m_DeltaTime;
	}
	m_RotateSpeed = glm::clamp(m_RotateSpeed, MIN_ROTATION_SPEED, MAX_ROTATION_SPEED);


	// Close window on escape key press
	if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(m_Window, true);
	}

	// Enable painting mode
	if (Input::isKeyPressed(GLFW_KEY_P)) {
		m_PaintingMode = !m_PaintingMode;
	}

	// Spawn cube
	if (Input::isKeyDown(GLFW_KEY_E)) {
		if (m_PaintingMode) {
			m_CubePositions.push_back(m_Camera.getSpawnPosition(SPAWN_DISTANCE));
		}
		else {
			if (Input::isKeyPressed(GLFW_KEY_E))
				m_CubePositions.push_back(m_Camera.getSpawnPosition(SPAWN_DISTANCE));
		}
	}

	// Mix value
	if (Input::isKeyDown(GLFW_KEY_UP)) {
		m_MixValue += MIX_VALUE_CHANGE_RATE * m_DeltaTime;
	}
	if (Input::isKeyDown(GLFW_KEY_DOWN)) {
		m_MixValue -= MIX_VALUE_CHANGE_RATE * m_DeltaTime;
	}
	m_MixValue = glm::clamp(m_MixValue, MIN_MIX_VALUE, MAX_MIX_VALUE);


	// Camera movement
	if (Input::isKeyDown(GLFW_KEY_W)) {
		m_Camera.processKeyboard(FORWARD, m_DeltaTime, speedMultiplier);
	}
	if (Input::isKeyDown(GLFW_KEY_S)) {
		m_Camera.processKeyboard(BACKWARD, m_DeltaTime, speedMultiplier);
	}
	if (Input::isKeyDown(GLFW_KEY_A)) {
		m_Camera.processKeyboard(LEFT, m_DeltaTime, speedMultiplier);
	}
	if (Input::isKeyDown(GLFW_KEY_D)) {
		m_Camera.processKeyboard(RIGHT, m_DeltaTime, speedMultiplier);
	}
}

void Window::handleMouseMovement(double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (m_FirstMouse) {
		m_LastX = xpos;
		m_LastY = ypos;
		m_FirstMouse = false;
	}

	float xoffset = xpos - static_cast<float>(m_LastX);
	float yoffset = static_cast<float>(m_LastY) - ypos;

	m_LastX = xpos;
	m_LastY = ypos;

	m_Camera.processMouseMovement(xoffset, yoffset);
}

void Window::handleMouseScroll(double xoffset, double yoffset) {
	m_Camera.processMouseScroll(static_cast<float>(yoffset));
}

void Window::glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->handleResize(width, height);
	}
}

void Window::glfwMouseCallback(GLFWwindow* window, double xpos, double ypos) {
	Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->handleMouseMovement(xpos, ypos);
	}
}

void Window::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->handleMouseScroll(xoffset, yoffset);
	}
}