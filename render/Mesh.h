#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>
#include <vector>

class RawVertex { // 顶点
public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texCorrd;
	glm::vec3 normal;

	RawVertex() = default;
	~RawVertex() = default;

	RawVertex(
		const glm::vec4& _pos,
		const glm::vec4& _color = glm::vec4(0, 0, 0, 0),
		const glm::vec2& _tex = glm::vec2(0, 0),
		const glm::vec3& _normal = glm::vec3(0, 0, 1)
	) : position(_pos), color(_color), texCorrd(_tex), normal(_normal) { }
};

class Vertex {
public:
	glm::vec4 worldPos;
	glm::vec4 windowPos;
	glm::vec4 color;
	glm::vec2 texCoord;
	glm::vec3 normal;

	Vertex() = default;
	~Vertex() = default;

	Vertex(
		const glm::vec4& _worldPos,
		const glm::vec4& _windowPos,
		const glm::vec4& _color,
		const glm::vec2& _texCoord,
		const glm::vec3& _normal
	) : worldPos(_worldPos), windowPos(_windowPos), color(_color), texCoord(_texCoord), normal(_normal) { }

	Vertex(const Vertex& v) : worldPos(v.worldPos), windowPos(v.windowPos), color(v.color), texCoord(v.texCoord), normal(v.normal) { }

};

class Mesh {
public:
	std::vector<RawVertex> VBO; // 顶点缓冲对象
	std::vector<unsigned int> EBO; // 索引缓冲对象

	Mesh() = default;
	Mesh(const int& vNum, const int iNum);
	Mesh(const Mesh& mesh) : VBO(mesh.VBO), EBO(mesh.EBO) { }
	~Mesh() = default;

	Mesh& operator=(const Mesh& mesh);
	Mesh& operator+=(const Mesh& mesh);
	void addMesh(const Mesh& mesh);
	void addTriangle(const RawVertex& v1, const RawVertex& v2, const RawVertex& v3);
};

#endif // !__MESH_H__

