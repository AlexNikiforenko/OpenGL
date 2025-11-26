#include "Camera.h"

// vector constructor
Camera::Camera(glm::vec3 m_Position, glm::vec3 m_Up, float m_Yaw, float m_Pitch)
	: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED),
	m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM), m_Position(m_Position),
	m_WorldUp(m_Up), m_Yaw(YAW), m_Pitch(PITCH) {
	updateCameraVectors();
}

// scalar values constructor
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float m_Yaw, float m_Pitch)
	: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM),
	m_Position(posX, posY, posZ), m_WorldUp(upX, upY, upZ), m_Yaw(YAW), m_Pitch(PITCH) {
	updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, float m_DeltaTime, float speedMultiplier) {
	float velocity = m_MovementSpeed * m_DeltaTime * speedMultiplier;
	switch (direction) {
	case FORWARD:
		m_Position += m_Front * velocity;
		break;
	case BACKWARD:
		m_Position -= m_Front * velocity;
		break;
	case LEFT:
		m_Position -= m_Right * velocity;
		break;
	case RIGHT:
		m_Position += m_Right * velocity;
		break;
	}
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw   += xoffset;
	m_Pitch += yoffset;

	if (constrainPitch) {
		m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
	}
	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
	m_Zoom -= yoffset;
	m_Zoom = glm::clamp(m_Zoom, 1.0f, 45.0f);
}

void Camera::setMovSpeed(float speed) {
	m_MovementSpeed = speed;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::vec3 Camera::getSpawnPosition(float spawnDistance) {
	return m_Position + m_Front * spawnDistance;
}

void Camera::updateCameraVectors() {
	glm::vec3 front_vec;
	front_vec.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front_vec.y = sin(glm::radians(m_Pitch));
	front_vec.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Front = glm::normalize(front_vec);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}
