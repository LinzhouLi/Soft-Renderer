#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <glm/glm.hpp>

class Texture;
class Shader;

class Material {
public:
	glm::vec4 color;
	glm::vec4 specular;
	int gloss;
	Texture* texture;
	Shader* shader;

	Material(Shader* sha = nullptr, Texture* tex = nullptr, const int& _gloss = 32);
	Material(const glm::vec4& _color, const glm::vec4& _specular, const int& _gloss);
	~Material() = default;

	void setShader(Shader* sha);
	void setTexture(Texture* tex);
};

#endif // !__Material_H__

