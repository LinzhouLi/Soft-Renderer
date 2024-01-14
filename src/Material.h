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

	Material(Shader* sha, Texture* tex, const int& _gloss = 32);
	Material(
		Shader* sha,
		const glm::vec4& _color = glm::vec4(1.0, 1.0, 1.0, 1.0),
		const int& _gloss = 32
	);
	~Material() = default;

	void setShader(Shader* sha);
	void setTexture(Texture* tex);
};

#endif // !__Material_H__

