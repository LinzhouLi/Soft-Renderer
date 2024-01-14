#include "Mesh.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


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
	VBO.insert(VBO.end(), mesh.VBO.begin(), mesh.VBO.end()); // 加入mesh中所有VBO
	EBO.reserve(EBO.size() + mesh.EBO.size()); // 增加EBO vector预留空间
	const unsigned int* p = mesh.EBO.data();
	for (int i = 0; i < mesh.EBO.size(); i++) { // 加入mesh中所有EBO
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


void Mesh::loadObj(const string& filePath) {
	std::ifstream in(filePath);
	if (!in) {
		std::cout << "Fail to load file: " << filePath << std::endl;
		return;
	}
	VBO.clear();
	EBO.clear();

	std::string line, flagStr;

	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line);
		iss >> flagStr;
		if (flagStr == "v") { // 顶点坐标
			glm::vec3 v;
			iss >> v.x >> v.y >> v.z;
			RawVertex vertex(v);
			VBO.push_back(vertex);
		}
		else if (flagStr == "vt") { // 贴图坐标
			glm::vec2 vt;
			iss >> vt.x >> vt.y;
			vt.y = 1 - vt.y;
			texCoords.push_back(vt);
		}
		else if (flagStr == "vn") { // 法向量
			glm::vec3 vn;
			iss >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
		else if (flagStr == "f") { // 三角面对应三个顶点的信息
			int vIndex, vtIndex, vnIndex;
			char bar; // 分隔符'/'
			for (int i = 0; i < 3; i++) {
				iss >> vIndex >> bar >> vtIndex >> bar >> vnIndex;
				if (vIndex > VBO.size() || vtIndex > texCoords.size() || vnIndex > normals.size()) {
					std::cout << "Fail to load file: " << filePath << std::endl;
					return;
				}
				VBO[vIndex - 1].texCorrd = texCoords[vtIndex - 1];
				VBO[vIndex - 1].normal = normals[vnIndex - 1];
				EBO.push_back(vIndex - 1);
			}
		}
	}
	in.close();
}