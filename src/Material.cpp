#include "Material.h"

#include "Texture.h"
#include "Shader.h"

Material::Material(Shader* sha, Texture* tex, const int& _gloss) {
	color = glm::vec4(255, 255, 255, 255);
	specular = glm::vec4(255, 255, 255, 255);
	gloss = _gloss;
	texture = tex;
	shader = sha;
}

Material::Material(const glm::vec4& _color, const glm::vec4& _specular, const int& _gloss)
	: color(_color), specular(_specular), gloss(_gloss) {
	texture = nullptr;
	shader = nullptr;
}

void Material::setShader(Shader* sha) {
	shader = sha;
}

void Material::setTexture(Texture* tex) {
	texture = tex;
}