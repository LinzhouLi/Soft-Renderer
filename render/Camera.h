#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	float fov;
	float aspect;

	float pitch; // ¸©Ñö
	float yaw; // Æ«º½

	float n; // near
	float f; // far

	Camera(
		const glm::vec3& pos = glm::vec3(0.0f, 0.0f, -1.0f),
		const glm::vec3& worldup = glm::vec3(0.0f, 1.0f, 0.0f),
		const glm::vec3& lookAt = glm::vec3(0.0f, 0.0f, 0.0f),
		const float& _fov = 60.0,
		const float& _n = 0.3f,
		const float& _f = 100.0f
	);

	void updateAspact(const int& width, const int& height);
	void updateFov(const float& f);
	glm::mat4 viewMatrix();
	glm::mat4 perspectiveMatrix();

	void moveForward(const float& distance);
	void moveBackward(const float& distance);
	void moveRight(const float& distance);
	void moveLeft(const float& distance);
	void moveUp(const float& distance);
	void moveDown(const float& distance);

	void rotatePitch(const float& angle); // ¸©Ñö
	void rotateYaw(const float& angle); // Æ«º½

	void updateVectors();
};

#endif // !__CAMERA_H__

