#include "Light.h"

#include <algorithm>
#include "Math.h"

Light::Light(
	const glm::vec3& pos,
	const glm::vec3& _color,
	const glm::vec3& _specular,
	const glm::vec3& dir,
	const float& i
) {
	position = pos;
	direction = glm::normalize(dir);
	color = _color;
	color /= 255.0f;
	specular = _specular;
	specular /= 255.0f;
	intensity = i;
}

DirectionLight::DirectionLight(
	const glm::vec3& dir,
	const glm::vec3& _color,
	const glm::vec3& _specular,
	const float& i
) {
	direction = glm::normalize(dir);
	color = _color;
	color /= 255.0f;
	specular = _specular;
	specular /= 255.0f;
	intensity = i;
}

glm::vec3 DirectionLight::calculateColor(
	const glm::vec3& worldPos,
	const glm::vec3& cameraPos,
	const glm::vec3& normal,
	const glm::vec3& albedo,
	const int& gloss
) {
	glm::vec3 norm = glm::normalize(normal); // 片段法向量
	float diff = glm::dot(norm, -direction);
	glm::vec3 reflectDir = reflect(direction, norm); // 反射光方向向量
	glm::vec3 viewDir = glm::normalize(cameraPos - worldPos); // 观察方向向量
	float spec = pow(glm::dot(reflectDir, viewDir), gloss);

	if (diff < 0 || spec < 0) {
		return albedo;
	}
	glm::vec3 diffuse = color * diff * (albedo / 255.0f);
	glm::vec3 specul = specular * spec;
	return (diffuse + specul) * intensity * 255.0f;
}