#include "Scene.h"

Scene::Scene() {
    m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_ClearColor = glm::vec3(0.2f, 0.3f, 0.3f);
    m_CubeColor = glm::vec3(1.0, 0.5, 0.2);

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
}