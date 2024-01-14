#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <glm/glm.hpp>

class Mesh;
class Material;

class Object {
public:
	Mesh* mesh;
	Material* material;

	Object() = default;
	~Object() = default;
	Object(Mesh* m, Material* mat) : mesh(m), material(mat) { }

	void setMesh(Mesh* m) { mesh = m; }
	void setMaterial(Material* mat) { material = mat; }

	void transform(const glm::mat4& m);
};

#endif // !__OBJECT_H__

