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
};

#endif // !__LIGHT_H__

