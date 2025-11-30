#include "Camera.h"

#include <glm/gtc/quaternion.hpp>

// vector constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float roll)
	   : m_Position(position),
		 m_WorldUp(up),
		 m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		 m_Yaw(yaw), 
		 m_Pitch(pitch) 
{
	updateCameraVectors();
}

// scalar values constructor
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float roll)
	: Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch)
{
	
}

void Camera::processKeyboard(CameraMovement direction, float m_DeltaTime, float speedMultiplier) {
	float velocity = m_MovementSpeed * m_DeltaTime * speedMultiplier;
	switch (direction) {
	case CameraMovement::FORWARD:
		m_Position += m_Front * velocity;
		break;
	case CameraMovement::BACKWARD:
		m_Position -= m_Front * velocity;
		break;
	case CameraMovement::LEFT:
		m_Position -= m_Right * velocity;
		break;
	case CameraMovement::RIGHT:
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

void Camera::processRoll(float rollOffset) {
	m_Roll += rollOffset;
	updateCameraVectors();
}

void Camera::setMovSpeed(float speed) {
	m_MovementSpeed = speed;
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::vec3 Camera::getSpawnPosition(float spawnDistance) {
	return m_Position + m_Front * spawnDistance;
}


// TODO: fix roll bug
void Camera::updateCameraVectors() {
	glm::vec3 front_vec;
	front_vec.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front_vec.y = sin(glm::radians(m_Pitch));
	front_vec.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Front = glm::normalize(front_vec);

	glm::vec3 rightBase = glm::normalize(glm::cross(m_Front, m_WorldUp));
	glm::vec3 upBase = glm::normalize(glm::cross(rightBase, m_Front));

	glm::quat rotationQuat = glm::angleAxis(glm::radians(m_Roll), m_Front);

	m_Right = glm::normalize(rotationQuat * rightBase);
	m_Up = glm::normalize(rotationQuat * upBase);

	printf("ROLL: %.2f\n", m_Roll);
}
