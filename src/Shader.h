#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/glm.hpp>

class Vertex;
class RawVertex;
class Scene;

class Shader {
private:
	Scene* scene;
	glm::mat4 modelMatrix;

public:
	Shader();
	~Shader() = default;

	Vertex vertexShader(const RawVertex& v);
	glm::vec4 fragmentShader(const Vertex& v2f);

	void setModelMatrix(const glm::mat4& m);
	void setScene(Scene* s);
};

#endif // !__SHADER_H__

