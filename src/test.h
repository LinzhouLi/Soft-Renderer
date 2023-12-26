#ifndef __TEST_H__
#define __TEST_H__

#include "Mesh.h"

Mesh createPlane(const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal) {
	Mesh result(4, 6);
	result.VBO[0].position = glm::vec4(leftTop, 1.0f);
	result.VBO[0].normal = normal;
	result.VBO[0].color = glm::vec4(255, 0, 0, 255);
	result.VBO[0].texCorrd = glm::vec2(0.0f, 1.0f);
	result.VBO[1].position = glm::vec4(rightTop, 1.0f);
	result.VBO[1].normal = normal;
	result.VBO[1].color = glm::vec4(0, 255, 0, 255);
	result.VBO[1].texCorrd = glm::vec2(1.0f, 1.0f);
	result.VBO[2].position = glm::vec4(rightBottom, 1.0f);
	result.VBO[2].normal = normal;
	result.VBO[2].color = glm::vec4(0, 0, 255, 255);
	result.VBO[2].texCorrd = glm::vec2(1.0f, 0.0f);
	result.VBO[3].position = glm::vec4(leftBottom, 1.0f);
	result.VBO[3].normal = normal;
	result.VBO[3].color = glm::vec4(255, 0, 255, 255);
	result.VBO[3].texCorrd = glm::vec2(0.0f, 0.0f);

	result.EBO[0] = 0;
	result.EBO[1] = 2;
	result.EBO[2] = 1;
	result.EBO[3] = 0;
	result.EBO[4] = 3;
	result.EBO[5] = 2;

	return result;
}

Mesh createBox(const glm::vec3& center, float radius) {
	Mesh result;
	
	Mesh front = createPlane(
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(radius, -radius, radius),
		center + glm::vec3(radius, radius, radius),
		glm::vec3(0, 0, 1)
	);
	result.addMesh(front);

	Mesh left = createPlane(
		center + glm::vec3(-radius, radius, -radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(-radius, radius, radius),
		glm::vec3(-1, 0, 0)
	);
	result.addMesh(left);

	Mesh right = createPlane(
		center + glm::vec3(radius, radius, radius),
		center + glm::vec3(radius, -radius, radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(radius, radius, -radius),
		glm::vec3(1, 0, 0)
	);
	result.addMesh(right);
	
	Mesh back = createPlane(
		center + glm::vec3(radius, radius, -radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(-radius, radius, -radius),
		glm::vec3(0, 0, -1)
	);
	result.addMesh(back);
	
	Mesh up = createPlane(
		center + glm::vec3(-radius, radius, -radius),
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(radius, radius, radius),
		center + glm::vec3(radius, radius, -radius),
		glm::vec3(0, 1, 0)
	);
	result.addMesh(up);

	Mesh down = createPlane(
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(radius, -radius, radius),
		glm::vec3(0, -1, 0)
	);
	result.addMesh(down);
	
	return result;
}

#endif // !__TEST_H__

