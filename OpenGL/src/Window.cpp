#include "Window.h"

#include "CubeVertices.h"
#include "Input.h"
#include "Texture.h"

#include "stb_image.h"
#include <iostream>


/*
	// Remove paint traces
	// Choose texture
	// Collisions
	// Controller support
	// Fix roll vectors (opposite directions)

	// Add scene class
	// Add renderer class
	// Add cube renderer class
	// Add BW shader for screamer
	// Try color light

	// TODO: Clean a LOT of SHIT
	// shaders, VAOs, VBOs

	// TODO: Add ImGui for lightning
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

	m_Shader = Shader("resources/shaders/shader.vert", "resources/shaders/shader.frag");
	m_LightShader = Shader("resources/shaders/light_cube.vert", "resources/shaders/light_cube.frag");

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

	glGenVertexArrays(1, &m_CubeVAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_CubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalTexVerts), normalTexVerts, GL_STATIC_DRAW);

	

	const int STRIDE = 8 * sizeof(float);

	// Position attribute (3 float)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute (3 float)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//  Texture attribute (2 float) 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Light's VAO
	glGenVertexArrays(1, &m_LightVAO);
	glBindVertexArray(m_LightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
	glEnableVertexAttribArray(0);


	// Load and create texture1
	m_Texture1 = std::make_unique<Texture>("resources/textures/image.jpg");
	m_Texture2 = std::make_unique<Texture>("resources/textures/screamer.jpg");

	m_Shader.use();
	m_Shader.setInt("texture1", 0);
	m_Shader.setInt("texture2", 1);

	glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	glDeleteVertexArrays(1, &m_CubeVAO);
	glDeleteVertexArrays(1, &m_LightVAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Window::run() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

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
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool isMovingLight = true;

	float lightSpeed = 0.75f;
	float lightRadius = 5.0f;
	float time = static_cast<float>(glfwGetTime());
	float lightX = sin(lightSpeed) * lightRadius;
	float lightZ = cos(lightSpeed) * lightRadius;
	if (isMovingLight) {
		lightX = sin(time * lightSpeed) * lightRadius;
		lightZ = cos(time * lightSpeed) * lightRadius;
	}
	glm::vec3 lightPos = glm::vec3(lightX, 1.0f, lightZ);

	glm::mat4 projection = glm::perspective(glm::radians(m_Camera.getZoom()),
		static_cast<float>(m_Width) / static_cast<float>(m_Height), 0.1f, 100.0f);
	glm::mat4 view = m_Camera.getViewMatrix();

	m_Shader.use();

	m_Shader.setMatrix("projection", projection);
	m_Shader.setMatrix("view", view);
	m_Shader.setFloat("mixValue", m_MixValue);
	m_Shader.setFloat("time", time);

	m_Shader.setVec3f("lightPos", lightPos);
	m_Shader.setVec3f("lightColor", glm::vec3(1.0f));
	m_Shader.setVec3f("viewPos", m_Camera.getPosition());

	m_Texture1->bind(0);
	m_Texture2->bind(1);
	

	glBindVertexArray(m_CubeVAO);
	for (size_t i = 0; i < m_CubePositions.size(); i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_CubePositions[i]);
		float angle = 20.0f * i + 10.0f;
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle) * m_RotateSpeed, glm::vec3(1.0f, 0.3f, 0.5f));
		m_Shader.setMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	m_LightShader.use();
	m_LightShader.setMatrix("projection", projection);
	m_LightShader.setMatrix("view", view);
	m_LightShader.setVec3f("objectColor", glm::vec3(1.0f));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.3f));

	m_LightShader.setMatrix("model", model);

	glBindVertexArray(m_LightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	ImGui::Begin("ImGUI window");
	ImGui::Checkbox("Moving light", &isMovingLight);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::handleResize(int width, int height) {
	m_Width = width;
	m_Height = height;

	glViewport(0, 0, width, height);
}

void Window::processInput() {
	Input::update(m_Window);

	ImGuiIO& io = ImGui::GetIO();

	float speedMultiplier = 1.0f;
	float rollOffset = 0.0f;

	// Close window on escape key press
	if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(m_Window, true);
	}

	// Tab switch between ImGUI and Camera mode
	if (Input::isKeyPressed(GLFW_KEY_TAB)) {
		if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	// If ImGui active -> disable other processing
	if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
		return;
	}
	// ImGui focus
	if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
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

		// Enable painting mode
		if (Input::isKeyPressed(GLFW_KEY_P)) {
			m_PaintingMode = !m_PaintingMode;
		}

		// Spawn cube
		if (Input::isKeyDown(GLFW_KEY_F)) {
			if (m_PaintingMode) {
				m_CubePositions.push_back(m_Camera.getSpawnPosition(SPAWN_DISTANCE));
			}
			else {
				if (Input::isKeyPressed(GLFW_KEY_F))
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
			m_Camera.processKeyboard(CameraMovement::FORWARD, m_DeltaTime, speedMultiplier);
		}
		if (Input::isKeyDown(GLFW_KEY_S)) {
			m_Camera.processKeyboard(CameraMovement::BACKWARD, m_DeltaTime, speedMultiplier);
		}
		if (Input::isKeyDown(GLFW_KEY_A)) {
			m_Camera.processKeyboard(CameraMovement::LEFT, m_DeltaTime, speedMultiplier);
		}
		if (Input::isKeyDown(GLFW_KEY_D)) {
			m_Camera.processKeyboard(CameraMovement::RIGHT, m_DeltaTime, speedMultiplier);
		}
		if (Input::isKeyDown(GLFW_KEY_Q)) {
			rollOffset -= ROLL * m_DeltaTime;
		}
		if (Input::isKeyDown(GLFW_KEY_E)) {
			rollOffset += ROLL * m_DeltaTime;
		}

		if (rollOffset != 0.0f) {
			m_Camera.processRoll(rollOffset);
		}
	}
}

void Window::handleMouseMovement(double xposIn, double yposIn) {
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse || glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		m_FirstMouse = true;
		return;
	}

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