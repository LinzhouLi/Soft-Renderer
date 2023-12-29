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

	Material(Texture* tex = nullptr, const int& _gloss = 32);
	Material(const glm::vec4& _color = glm::vec4(255, 255, 255, 255), const int& _gloss = 32);
	~Material() = default;

	void setShader(Shader* sha);
	void setTexture(Texture* tex);
};

#endif // !__Material_H__

