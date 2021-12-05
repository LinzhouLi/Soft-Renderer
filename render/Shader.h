#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/glm.hpp>
#include "Vertex.h"

class Shader {
private:
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectMatrix;
	glm::mat4 viewPortMatrix;

public:
	Shader() = default;
	~Shader() = default;

	Vertex vertexShader(const RawVertex& v);
	glm::vec4 fragmentShader(const Vertex& v2f);

	void setViewPortMatrix(const glm::mat4& m);
	void setModelMatrix(const glm::mat4& m);
	void setViewMatrix(const glm::mat4& m);
	void setProjectMatrix(const glm::mat4& m);
};

Shader::Shader() {
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
	projectMatrix = glm::mat4(1.0f);
}

Vertex Shader::vertexShader(const RawVertex& v) {
	Vertex result;
	result.worldPos = modelMatrix * v.position;
	result.windowPos = viewPortMatrix * projectMatrix * viewMatrix * result.worldPos;
	result.color = v.color;
	result.normal = v.normal;
	result.texCoord = v.texCorrd;
	return result;
}

glm::vec4 Shader::fragmentShader(const Vertex& v2f) {
	glm::vec4 color = v2f.color;
	return color;
}

void Shader::setViewPortMatrix(const glm::mat4& m) {
	viewPortMatrix = m;
}

void Shader::setModelMatrix(const glm::mat4& m) {
	modelMatrix = m;
}

void Shader::setViewMatrix(const glm::mat4& m) {
	viewMatrix = m;
}

void Shader::setProjectMatrix(const glm::mat4& m) {
	projectMatrix = m;
}

#endif // !__SHADER_H__

