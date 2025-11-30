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

namespace CameraDefaults {
	inline constexpr float yaw           = -90.0f;
	inline constexpr float pitch         =  0.0f;

	inline constexpr float rollSpeed     = 100.0f;

	inline constexpr float speed         = 2.5f;
	inline constexpr float sensitivity   = 0.1f;
	inline constexpr float zoom          = 45.0f;
};

class Camera {
public:
	// Vector constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = CameraDefaults::yaw, float pitch = CameraDefaults::pitch, float roll = CameraDefaults::rollSpeed);

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
	float m_Yaw = CameraDefaults::yaw;
	float m_Pitch = CameraDefaults::pitch;
	float m_Roll = 0.0f;

	// Camera options
	float m_MovementSpeed = CameraDefaults::speed;
	float m_MouseSensitivity = CameraDefaults::sensitivity;
	float m_Zoom = CameraDefaults::zoom;
};