#include "Shader.h"

#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"

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
	result.z = 1 / result.windowPos.w; // 处理纹理透视映射
	result.worldPos *= result.z;
	result.color = v.color * result.z;
	result.normal = v.normal * result.z;
	result.texCoord = v.texCorrd * result.z;
	return result;
}

glm::vec4 Shader::fragmentShader(const Vertex& v) {
	glm::vec3 resultColor = glm::vec3(
		this->scene->currentMaterial->color.r,
		this->scene->currentMaterial->color.g,
		this->scene->currentMaterial->color.b
	);
	if (this->scene->currentMaterial->texture != nullptr)
		resultColor = this->scene->currentMaterial->texture->sample2D(v.texCoord);
	for (auto light : this->scene->lights)
		resultColor = light->calculateColor(v.worldPos, this->scene->camera->position, v.normal, resultColor, this->scene->currentMaterial->gloss);
	return glm::vec4(resultColor, 1.0f);
}

void Shader::setScene(Scene* s) {
	this->scene = s;
}