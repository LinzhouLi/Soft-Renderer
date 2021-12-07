#include "Shader.h"

#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

Shader::Shader() {
	modelMatrix = glm::mat4(1.0f);
}

void Shader::setModelMatrix(const glm::mat4& m) {
	modelMatrix = m;
}

Vertex Shader::vertexShader(const RawVertex& v) {
	Vertex result;
	result.worldPos = modelMatrix * v.position;
	result.windowPos = this->scene->projectMatrix * this->scene->viewMatrix * result.worldPos;
	result.z = 1 / result.windowPos.w;
	result.worldPos *= result.z;
	result.color = v.color * result.z;
	result.normal = v.normal * result.z;
	result.texCoord = v.texCorrd * result.z;
	return result;
}

glm::vec4 Shader::fragmentShader(const Vertex& v) {
	if (this->scene->currentMaterial->texture == nullptr) {
		return v.color;
	}
	else {
		return this->scene->currentMaterial->texture->sample2D(v.texCoord);
	}
}

void Shader::setScene(Scene* s) {
	this->scene = s;
}