#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
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
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = YAW, float pitch = PITCH);

	// scalar values constructor
	Camera(float posX, float posY, 
		   float posZ, float upX, 
		   float upY,  float upZ, 
		   float yaw,  float pitch);

	glm::mat4 getViewMatrix();

	void processKeyboard(CameraMovement direction, float deltaTime);

	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	void processMouseScroll(float yoffset);

	float getYaw() const { return yaw; };
	float getPitch() const { return pitch; };

	float getMovSpeed() const { return movementSpeed; };
	float getMouseSens() const { return mouseSensitivity; };
	float getZoom() const { return zoom; };
	

private:
	void updateCameraVectors();

	// camera attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// euler angles
	float yaw;
	float pitch;

	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;
};