#pragma once

#include <glm/glm.hpp>
#include <vector>

class Scene
{
public:
	Scene();

	const glm::vec3& getLightPos() const { return m_LightPos; }
	void setLightPos(const glm::vec3& pos) { m_LightPos = pos; }

    const glm::vec3& getLightColor() const { return m_LightColor; }
    void setLightColor(const glm::vec3& color) { m_LightColor = color; }

    bool getIsMovingLight() const { return m_IsMovingLight; }
    void setIsMovingLight(bool moving) { m_IsMovingLight = moving; }

    bool getPaintingMode() const { return m_PaintingMode; }
    void setPaintingMode(bool mode) { m_PaintingMode = mode; }

    float getLightSpeed() const { return m_LightSpeed; }
    void setLightSpeed(float speed) { m_LightSpeed = speed; }

    float getLightRadius() const { return m_LightRadius; }
    void setLightRadius(float radius) { m_LightRadius = radius; }

    const glm::vec3& getClearColor() const { return m_ClearColor; }
    void setClearColor(const glm::vec3& color) { m_ClearColor = color; }

    float getMixValue() const { return m_MixValue; }
    void setMixValue(float value) { m_MixValue = value; }

    float getRotateSpeed() const { return m_RotateSpeed; }
    void setRotateSpeed(float speed) { m_RotateSpeed = speed; }

	glm::vec3 getCubeColor() const { return m_CubeColor; }
	void setCubeColor(const glm::vec3& color) { m_CubeColor = color; }

    const std::vector<glm::vec3>& getCubePositions() const { return m_CubePositions; }

    void addCubePosition(const glm::vec3& pos) { m_CubePositions.push_back(pos); }
private:
    glm::vec3 m_LightPos;
	glm::vec3 m_LightColor;
	bool m_IsMovingLight = true;
	float m_LightSpeed = 0.75f;
	float m_LightRadius = 5.0f;

	glm::vec3 m_ClearColor;
	float m_MixValue = 0.0f;
	float m_RotateSpeed = 0.1f;

    glm::vec3 m_CubeColor;

    bool m_PaintingMode = false;


	std::vector<glm::vec3> m_CubePositions;
};

