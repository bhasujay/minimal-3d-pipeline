#include "Camera.h"
#include <cmath>

Camera::Camera(Vec3 startPosition, float startYaw, float startPitch)
	: position(startPosition),
	  worldUp(Vec3(0.0f, 1.0f, 0.0f)),
	  yaw(startYaw),
	  pitch(startPitch),
	  movementSpeed(5.0f),
	  mouseSensitivity(0.1f),
	  zoom(45.0f),
	  firstMouse(true),
	  lastMouseX(0.0),
	  lastMouseY(0.0) {
	updateCameraVectors();
}

void Camera::processKeyboard(Movement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position = position + front * velocity;
	if (direction == BACKWARD)
		position = position - front * velocity;
	if (direction == LEFT)
		position = position - right * velocity;
	if (direction == RIGHT)
		position = position + right * velocity;
}

void Camera::processMouseMovement(double xpos, double ypos) {
	if (firstMouse) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
		return;
	}

	float xoffset = static_cast<float>(xpos - lastMouseX);
	float yoffset = static_cast<float>(lastMouseY - ypos); // Reversed: screen Y increases downward

	lastMouseX = xpos;
	lastMouseY = ypos;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Clamp pitch to prevent flipping
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	updateCameraVectors();
}

Mat4 Camera::getViewMatrix() const {
	return Mat4::lookAt(
		position.x, position.y, position.z,
		position.x + front.x, position.y + front.y, position.z + front.z,
		up.x, up.y, up.z
	);
}

void Camera::updateCameraVectors() {
	// Calculate the new Front vector
	Vec3 newFront;
	newFront.x = cosf(yaw * (3.14159265f / 180.0f)) * cosf(pitch * (3.14159265f / 180.0f));
	newFront.y = sinf(pitch * (3.14159265f / 180.0f));
	newFront.z = sinf(yaw * (3.14159265f / 180.0f)) * cosf(pitch * (3.14159265f / 180.0f));
	front = newFront.normalized();

	// Also re-calculate the Right and Up vector
	right = Vec3::cross(front, worldUp).normalized();
	up = Vec3::cross(right, front).normalized();
}
