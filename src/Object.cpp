#include "Object.h"

#include "mesh.h"
#include "Material.h"
#include "Shader.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using std::string;

void Object::transform(const glm::mat4& m) {
	material->shader->setModelMatrix(m);
}

void Object::loadObj(const string& filePath) {
	std::ifstream in(filePath);
	if (!in) {
		std::cout << filePath << "文件打开错误!" << std::endl;
		return;
	}
	this->mesh->VBO.clear();
	this->mesh->EBO.clear();

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
			this->mesh->VBO.push_back(vertex);
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
				if (vIndex > this->mesh->VBO.size() || vtIndex > texCoords.size() || vnIndex > normals.size()) {
					std::cout << filePath << "文件出错!" << std::endl;
					return;
				}
				this->mesh->VBO[vIndex - 1].texCorrd = texCoords[vtIndex - 1];
				this->mesh->VBO[vIndex - 1].normal = normals[vIndex - 1];
				this->mesh->EBO.push_back(vIndex - 1);
			}
		}
	}
	in.close();
}