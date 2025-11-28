//#pragma once
//
//#include "Scene.h"
//#include "Camera.h"
//#include "Shader.h"
//#include "Texture.h"
//
//#include <vector>
//
//class Renderer
//{
//public:
//	Renderer() = default;
//	Renderer(Shader& shader);
//	~Renderer();
//
//	void beginFrame();
//	void endFrame();
//
//	void render(const Camera& camera, const std::vector<glm::vec3>& cubePositions, Texture* texture1, Texture* texture2, float mixValue, float rotateSpeed);
//	void drawCube(const glm::mat4& modelMatrix);
//private:
//	void setupCubeData();
//
//	Shader m_Shader;
//	unsigned int m_CubeVAO = 0;
//	unsigned int m_CubeVBO = 0;
//};
//
