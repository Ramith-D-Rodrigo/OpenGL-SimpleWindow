#include "Camera.h"

Camera::Camera() { }

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) {
	this->position = startPosition;
	this->worldUp = startUp;
	this->yaw = startYaw;
	this->pitch = startPitch;

	this->front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->moveSpeed = startMoveSpeed;
	this->turnSpeed = startTurnSpeed;

	update();
}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);	//remove the magnitute (just need the direction)

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = moveSpeed * deltaTime;
	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {	//look up limit
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {	//look down limit
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::CalculateViewMatrix() {
	return glm::lookAt(position, position + front, up);

}

Camera::~Camera() {

}