#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float ROLL        =  100.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera {
public:
	// Vector constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = YAW, float pitch = PITCH, float roll = ROLL);

	// Scalar values constructor
	Camera(float posX, float posY,
		   float posZ, float upX,
		   float upY, float upZ,
		   float yaw, float pitch, float roll);

	void processKeyboard(CameraMovement direction, float m_DeltaTime, float speedMultiplier = 1.0f);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yoffset);
	void processRoll(float rollOffset);

	void setMovSpeed(float speed);

	glm::vec3 getPosition() const { return m_Position; }
	glm::mat4 getViewMatrix() const;
	glm::vec3 getSpawnPosition(float spawnDistance);

	float getYaw() const { return m_Yaw; }
	float getPitch() const { return m_Pitch; }
	float getRoll() const { return m_Roll; }

	float getMovSpeed() const { return m_MovementSpeed; }
	float getMouseSens() const { return m_MouseSensitivity; }
	float getZoom() const { return m_Zoom; }
	

private:
	void updateCameraVectors();

	// Camera attributes
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	// Euler angles
	float m_Yaw = YAW;
	float m_Pitch = PITCH;
	float m_Roll = 0.0f;

	// Camera options
	float m_MovementSpeed = SPEED;
	float m_MouseSensitivity = SENSITIVITY;
	float m_Zoom = ZOOM;
};