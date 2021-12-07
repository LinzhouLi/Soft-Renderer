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
		const float& n = 0.3f,
		const float& f = 100.0f
	);

	void updateAspact(const int& width, const int& height);
	void updateFov(const float& f);
	glm::mat4 viewMatrix();
	glm::mat4 perspectiveMatrix();
};

#endif // !__CAMERA_H__

