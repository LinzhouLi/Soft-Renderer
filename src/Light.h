#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>

class Light {
public:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 specular;
	glm::vec3 direction;
	float intensity;

	Light(
		const glm::vec3& pos = glm::vec3(0, 0, 0),
		const glm::vec3& _color = glm::vec3(255, 255, 255),
		const glm::vec3& _specular = glm::vec3(255, 255, 255),
		const glm::vec3& dir = glm::vec3(0, -1, 0),
		const float& i = 1.0f
	);

	virtual glm::vec3 calculateColor(
		const glm::vec3& worldPos,
		const glm::vec3& cameraPos,
		const glm::vec3& normal,
		const glm::vec3& color,
		const int& gloss
	) = 0;
};

class DirectionLight : public Light {
public:
	DirectionLight(
		const glm::vec3& dir = glm::vec3(0, -1, 1),
		const glm::vec3& _color = glm::vec3(255, 255, 255),
		const glm::vec3& _specular = glm::vec3(255, 255, 255),
		const float& i = 1.0
	);

	glm::vec3 calculateColor(
		const glm::vec3& worldPos,
		const glm::vec3& cameraPos,
		const glm::vec3& normal,
		const glm::vec3& color,
		const int& gloss
	);
};

#endif // !__LIGHT_H__

