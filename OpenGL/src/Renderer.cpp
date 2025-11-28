//#include "Renderer.h"
//
//#include "CubeVertices.h"
//
//#include <GLFW/glfw3.h>
//#include <iostream>
//
//Renderer::Renderer(Shader& shader) : m_Shader(shader)
//{
//	setupCubeData();
//}
//
//Renderer::~Renderer()
//{
//	glDeleteVertexArrays(1, &m_CubeVAO);
//	glDeleteBuffers(1, &m_CubeVBO);
//}
//
//void Renderer::beginFrame()
//{
//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}
//
//void Renderer::endFrame()
//{
//
//}
//
//void Renderer::render(const Camera& camera, const std::vector<glm::vec3>& cubePositions, Texture* texture1, Texture* texture2, float mixValue, float rotateSpeed)
//{
//	m_Shader.use();
//
//	float timeValue = static_cast<float>(glfwGetTime());
//	float offset = 0.5f;
//	float speed = 0.75f;
//
//	m_Shader.setFloat("xOffset", cos(speed * timeValue) * offset);
//	m_Shader.setFloat("yOffset", sin(25 * speed * timeValue) * offset);
//	m_Shader.setFloat("colorOffset", cos(timeValue / 2.0f));
//	m_Shader.setFloat("mixValue", mixValue);
//	m_Shader.setFloat("time", timeValue);
//
//	glm::mat4 projection = glm::mat4(2.0f);
//	projection = glm::perspective(glm::radians(camera.getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);
//
//	m_Shader.setMatrix("projection", projection);
//
//	texture1->bind(0);
//	texture2->bind(1);
//
//	glm::mat4 view = camera.getViewMatrix();
//	m_Shader.setMatrix("view", view);
//
//
//	glBindVertexArray(m_CubeVAO);
//	for (size_t i = 0; i < cubePositions.size(); i++) {
//		glm::mat4 model = glm::mat4(1.0f);
//		model = glm::translate(model, cubePositions[i]);
//		float angle = 20.0f * i + 10.0f;
//		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle) * rotateSpeed, glm::vec3(1.0f, 0.3f, 0.5f));
//		m_Shader.setMatrix("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
//}
//
//void Renderer::drawCube(const glm::mat4& modelMatrix)
//{
//
//}
//
//void Renderer::setupCubeData()
//{
//	glGenVertexArrays(1, &m_CubeVAO);
//	glGenBuffers(1, &m_CubeVBO);
//
//	glBindVertexArray(m_CubeVAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	// position attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	// texture attribute
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//}
