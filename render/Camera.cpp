#include "Camera.h"

#include "Math.h"

Camera::Camera(
	const glm::vec3& pos,
	const glm::vec3& worldup,
	const glm::vec3& lookAt,
	const float& _fov,
	const float& _n,
	const float& _f
) : position(pos), worldUp(worldup), n(_n), f(_f) {
	front = glm::normalize(lookAt - pos);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	fov = glm::radians(_fov);

	// Çó¸©Ñö½ÇºÍÆ«º½½Ç
	glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 frontXZ = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	float cosPitch = glm::dot(front, frontXZ);
	float cosYaw = glm::dot(worldFront, frontXZ);
	cosPitch = cosPitch > 1 ? 1 : cosPitch;
	cosPitch = cosPitch < 0 ? 0 : cosPitch;
	cosYaw = cosYaw > 1 ? 1 : cosYaw;
	cosYaw = cosYaw < -1 ? -1 : cosYaw;
	pitch = glm::degrees(acos(cosPitch));
	pitch = front.y > 0 ? pitch : -pitch;
	yaw = glm::degrees(acos(cosYaw));
	yaw = front.x > 0 ? yaw : 360 - yaw;
}

void Camera::updateAspact(const int& width, const int& height) {
	float w = width;
	float h = height;
	aspect = w / h;
}

void Camera::updateFov(const float& f) {
	fov = glm::radians(f);
}

glm::mat4 Camera::viewMatrix() {
	return getViewMatrix(position, front, right, up);
}

glm::mat4 Camera::perspectiveMatrix() {
	return getPerspectiveMatrix(fov, aspect, n, f);
}

void Camera::moveForward(const float& distance) {
	position += front * distance;
}

void Camera::moveBackward(const float& distance) {
	position -= front * distance;
}

void Camera::moveRight(const float& distance) {
	position += right * distance;
}

void Camera::moveLeft(const float& distance) {
	position -= right * distance;
}

void Camera::moveUp(const float& distance) {
	position += up * distance;
}

void Camera::moveDown(const float& distance) {
	position -= up * distance;
}

void Camera::rotatePitch(const float& angle) {
	pitch += angle;
	pitch = pitch > 89.0 ? 89.0 : pitch;
	pitch = pitch < -89.0 ? -89.0 : pitch;
	updateVectors();
}

void Camera::rotateYaw(const float& angle) {
	yaw += angle;
	while (yaw >= 360) yaw -= 360;
	while (yaw < 0) yaw += 360;
	updateVectors();
}

void Camera::updateVectors() {
	glm::vec3 _front;
	
	front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}