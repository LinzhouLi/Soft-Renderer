#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <glm/glm.hpp>
#include <string>
using std::string;

class Mesh;
class Material;

class Object {
public:
	Mesh* mesh;
	Material* material;

	Object() = default;
	~Object() = default;
	Object(Mesh* m, Material* mat) : mesh(m), material(mat) { }

	void transform(const glm::mat4& m);
	void loadObj(const string& filePath);
};

#endif // !__OBJECT_H__

