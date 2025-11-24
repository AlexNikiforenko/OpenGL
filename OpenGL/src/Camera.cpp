#pragma once

#include "Camera.h"

// vector constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
	mouseSensitivity(SENSITIVITY), zoom(ZOOM), position(position),
	worldUp(up), yaw(YAW), pitch(PITCH) {
	updateCameraVectors();
}

// scalar values constructor
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM),
	position(posX, posY, posZ), worldUp(upX, upY, upZ), yaw(YAW), pitch(PITCH) {
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	switch (direction) {
	case FORWARD:
		position += front * velocity;
		break;
	case BACKWARD:
		position -= front * velocity;
		break;
	case LEFT:
		position -= right * velocity;
		break;
	case RIGHT:
		position += right * velocity;
		break;
	}
	position.y = 0.0f;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw   += xoffset;
	pitch += yoffset;

	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch -= -89.0f;
	}
	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
	zoom -= yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors() {
	glm::vec3 front_vec;
	front_vec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front_vec.y = sin(glm::radians(pitch));
	front_vec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front_vec);

	right = glm::normalize(glm::cross(front, worldUp));
	up    = glm::normalize(glm::cross(right, front));
}
