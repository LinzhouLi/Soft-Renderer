#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "Vertex.h"

class Mesh {
public:
	std::vector<RawVertex> VBO; // ���㻺�����
	std::vector<unsigned int> EBO; // �����������

	Mesh() = default;
	Mesh(const int& vNum, const int iNum);
	Mesh(const Mesh& mesh) : VBO(mesh.VBO), EBO(mesh.EBO) { }
	~Mesh() = default;

	Mesh& operator=(const Mesh& mesh);
	Mesh& operator+=(const Mesh& mesh);
	void addMesh(const Mesh& mesh);
	void addTriangle(const RawVertex& v1, const RawVertex& v2, const RawVertex& v3);
};

Mesh::Mesh(const int& vNum, const int iNum) {
	VBO.resize(vNum);
	EBO.resize(iNum);
}

Mesh& Mesh::operator=(const Mesh& mesh) {
	if (&mesh != this) {
		return *this;
	}
	VBO = mesh.VBO;
	EBO = mesh.EBO;
	return *this;
}

Mesh& Mesh::operator+=(const Mesh& mesh) {
	addMesh(mesh);
	return *this;
}

void Mesh::addMesh(const Mesh& mesh) {
	int offset = VBO.size();
	VBO.insert(VBO.end(), mesh.VBO.begin(), mesh.VBO.end()); // ����mesh������VBO
	EBO.reserve(EBO.size() + mesh.EBO.size()); // ����EBO vectorԤ���ռ�
	const unsigned int* p = mesh.EBO.data();
	for (int i = 0; i < mesh.EBO.size(); i++) { // ����mesh������EBO
		EBO.push_back(offset + *(p + i));
	}
}

void Mesh::addTriangle(const RawVertex& v1, const RawVertex& v2, const RawVertex& v3) {
	int offset = VBO.size();
	VBO.push_back(v1);
	VBO.push_back(v2);
	VBO.push_back(v3);
	EBO.push_back(offset);
	EBO.push_back(offset + 1);
	EBO.push_back(offset + 2);
}

#endif // !__MESH_H__

