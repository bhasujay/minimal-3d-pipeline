#pragma once

#include "Geometry.h"

// Encapsulates an orbit camera controlled by mouse and keyboard input
class Camera {
public:
	enum Movement { FORWARD, BACKWARD, LEFT, RIGHT };

	Vec3 position;
	Vec3 front;
	Vec3 up;
	Vec3 right;
	Vec3 worldUp;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	bool firstMouse;
	double lastMouseX, lastMouseY;

	Camera(Vec3 startPosition = Vec3(0.0f, 2.0f, 8.0f), float startYaw = -90.0f, float startPitch = -15.0f);

	// Computes the view matrix
	Mat4 getViewMatrix() const;

	// Process keyboard input (WASD)
	void processKeyboard(Movement direction, float deltaTime);

	// Process mouse movement for looking around
	void processMouseMovement(double xpos, double ypos);

private:
	void updateCameraVectors();
};
