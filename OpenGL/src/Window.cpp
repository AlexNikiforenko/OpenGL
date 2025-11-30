#include "Window.h"

#include "Constants.h"
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

	// Add renderer class
	// Add cube renderer class
	// Add BW shader for screamer

	// TODO: Clean a LOT of SHIT
	// shaders, VAOs, VBOs

	TODO: Create SceneObject class to manage multiple scenes with different settings
	TODO: ResourceManager
	TODO: Mesh
	TODO: Renderer class
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

	// ImGui init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	glGenVertexArrays(1, &m_CubeVAO);
	glGenBuffers(1, &m_VBO);

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
	// Calculate light movements
	float lightX = 0.0f;
	float lightZ = 0.0f;
	float time = static_cast<float>(glfwGetTime());

	if (m_Scene.getIsMovingLight()) {
		float lightSpeed = m_Scene.getLightSpeed();
		float lightRadius = m_Scene.getLightRadius();

		lightX = sin(time * lightSpeed) * lightRadius;
		lightZ = cos(time * lightSpeed) * lightRadius;
	}
	else {
		lightX = 0.0f;
		lightZ = 0.0f;
	}
	glm::vec3 lightPos = glm::vec3(lightX, 1.0f, lightZ);
	m_Scene.setLightPos(lightPos);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_Scene.getLightPos());
	model = glm::scale(model, glm::vec3(0.3f));

	m_LightShader.setMatrix("model", model);
}

void Window::render() {
	glm::vec3 clearColor = m_Scene.getClearColor();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float time = static_cast<float>(glfwGetTime());

	glm::mat4 projection = glm::perspective(glm::radians(m_Camera.getZoom()),
		static_cast<float>(m_Width) / static_cast<float>(m_Height), 0.1f, 100.0f);
	glm::mat4 view = m_Camera.getViewMatrix();

	m_Shader.use();

	m_Shader.setMatrix("projection", projection);
	m_Shader.setMatrix("view", view);
	m_Shader.setFloat("mixValue", m_Scene.getMixValue());
	m_Shader.setFloat("time", time);

	m_Shader.setVec3f("lightPos", m_Scene.getLightPos());
	m_Shader.setVec3f("lightColor", m_Scene.getLightColor());
	m_Shader.setVec3f("viewPos", m_Camera.getPosition());
	m_Shader.setVec3f("objectColor", m_Scene.getCubeColor());

	m_Texture1->bind(0);
	m_Texture2->bind(1);
	
	const auto& cubePositions = m_Scene.getCubePositions();
	glBindVertexArray(m_CubeVAO);
	for (size_t i = 0; i < cubePositions.size(); i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i + 10.0f;
		model = glm::rotate(model, time * glm::radians(angle) * m_Scene.getRotateSpeed(), glm::vec3(1.0f, 0.3f, 0.5f));
		m_Shader.setMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	m_LightShader.use();
	m_LightShader.setMatrix("projection", projection);
	m_LightShader.setMatrix("view", view);
	m_LightShader.setVec3f("objectColor", m_Scene.getLightColor());

	glBindVertexArray(m_LightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	m_ImGuiLayer.renderGUI(m_Scene);

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
		float rotateSpeed = m_Scene.getRotateSpeed();
		float mixValue = m_Scene.getMixValue();
		bool paintingMode = m_Scene.getPaintingMode();
		
		
		// Speed boost when left shift is held
		if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
			speedMultiplier = RenderConfig::shiftBoost;
		}

		// Rotation speed control
		if (Input::isKeyDown(GLFW_KEY_RIGHT)) {
			rotateSpeed += RenderConfig::rotationSpeedRate * m_DeltaTime;
		}
		if (Input::isKeyDown(GLFW_KEY_LEFT)) {
			rotateSpeed -= RenderConfig::rotationSpeedRate * m_DeltaTime;
		}
		m_Scene.setRotateSpeed(glm::clamp(rotateSpeed, RenderConfig::minRotationSpeed, RenderConfig::maxRotationSpeed));

		// Enable painting mode
		if (Input::isKeyPressed(GLFW_KEY_P)) {
			m_Scene.setPaintingMode(!paintingMode);
		}

		// Spawn cube
		if (paintingMode) {
			if (Input::isKeyDown(GLFW_KEY_F)) {
				m_Scene.addCubePosition(m_Camera.getSpawnPosition(RenderConfig::spawnDistance));
			}
		}
		else {
			if (Input::isKeyPressed(GLFW_KEY_F)) {
				m_Scene.addCubePosition(m_Camera.getSpawnPosition(RenderConfig::spawnDistance));
			}
		}


		// Mix value
		if (Input::isKeyDown(GLFW_KEY_UP)) {
			mixValue += RenderConfig::mixValueRate * m_DeltaTime;
		}
		if (Input::isKeyDown(GLFW_KEY_DOWN)) {
			mixValue -= RenderConfig::mixValueRate * m_DeltaTime;
		}
		m_Scene.setMixValue(glm::clamp(mixValue, RenderConfig::minMixValue, RenderConfig::maxMixValue));


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
			rollOffset -= CameraDefaults::rollSpeed * m_DeltaTime;
		}
		if (Input::isKeyDown(GLFW_KEY_E)) {
			rollOffset += CameraDefaults::rollSpeed * m_DeltaTime;
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