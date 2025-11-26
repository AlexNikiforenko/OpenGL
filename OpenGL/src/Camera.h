#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SHIFT
};

// Default camera values
const float YAW         = -90.f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;

class Camera {
public:
	// vector constructor
	Camera(glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float m_Yaw = YAW, float m_Pitch = PITCH);

	// scalar values constructor
	Camera(float posX, float posY, 
		   float posZ, float upX, 
		   float upY,  float upZ, 
		   float m_Yaw,  float m_Pitch);

	void processKeyboard(CameraMovement direction, float m_DeltaTime, float speedMultiplier = 1.0f);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yoffset);

	void setMovSpeed(float speed);

	glm::mat4 getViewMatrix();
	glm::vec3 getSpawnPosition(float spawnDistance);

	float getYaw() const { return m_Yaw; }
	float getPitch() const { return m_Pitch; }

	float getMovSpeed() const { return m_MovementSpeed; }
	float getMouseSens() const { return m_MouseSensitivity; }
	float getZoom() const { return m_Zoom; }
	

private:
	void updateCameraVectors();

	// camera attributes
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	// euler angles
	float m_Yaw;
	float m_Pitch;

	// Camera options
	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;
};