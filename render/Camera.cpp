#include "Camera.h"

#include "Math.h"

Camera::Camera(
	const glm::vec3& pos,
	const glm::vec3& worldup,
	const glm::vec3& lookAt,
	const float& _n,
	const float& _f
) : position(pos), worldUp(worldup), n(_n), f(_f) {
	front = glm::normalize(lookAt - pos);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	// Çó¸©Ñö½ÇºÍÆ«º½½Ç

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