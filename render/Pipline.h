#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "Vertex.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Math.h"
#include "Mesh.h"

class Pipline {
private:
	int width;
	int height;
	FrameBuffer* frameBuffer;
	Shader* shader;

public:
	Pipline(const int& w, const int& h) : width(w), height(h), frameBuffer(nullptr), shader(nullptr) { }
	~Pipline();

	void init();
	void display();

	void resize(const int& w, const int& h);
	void fillColorBuffer(const glm::vec4& color);

	void setModelMatrix(const glm::mat4& m);
	void setViewMatrix(const glm::mat4& m);
	void setProjectMatrix(const glm::mat4& m);

	void scanLineFilling(const Vertex& left, const Vertex& right);
	void drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	void drawTriangleByBarycentric(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	void drawMesh(const Mesh& mesh);
};

Pipline::~Pipline() {
	if (frameBuffer) {
		delete frameBuffer;
	}
	if (shader) {
		delete shader;
	}
	frameBuffer = nullptr;
	shader = nullptr;
}

void Pipline::init() {
	if (frameBuffer) {
		delete frameBuffer;
	}
	if (shader) {
		delete shader;
	}
	glm::mat4 viewPortMatrix = getViewPortMatrix(0, 0, width, height);
	shader = new Shader;
	shader->setViewPortMatrix(viewPortMatrix);
	frameBuffer = new FrameBuffer(width, height);
}

void Pipline::display() {
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer->colorBuffer.data());
}

void Pipline::resize(const int& w, const int& h) {
	width = w;
	height = h;
	frameBuffer->resize(w, h);
	glm::mat4 viewPortMatrix = getViewPortMatrix(0, 0, w, h);
	shader->setViewPortMatrix(viewPortMatrix);
}

void Pipline::fillColorBuffer(const glm::vec4& color) {
	frameBuffer->fillColorBuffer(color);
}

void Pipline::setModelMatrix(const glm::mat4& m) {
	shader->setModelMatrix(m);
}

void Pipline::setViewMatrix(const glm::mat4& m) {
	shader->setViewMatrix(m);
}

void Pipline::setProjectMatrix(const glm::mat4& m) {
	shader->setProjectMatrix(m);
}

/*
* 光栅化三角形
* 使用重心坐标系, 遍历三角形包围盒填充
* 此算法易于插值, 但运行速度过慢, 不再使用
*/
void Pipline::drawTriangleByBarycentric(const Vertex& v1, const Vertex& v2, const Vertex& v3) {

	// 计算包围盒
	glm::vec2 min, max;
	min.x = triMin(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	min.y = triMin(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	max.x = triMax(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	max.y = triMax(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	// 遍历包围盒中的点
	glm::vec2 pos;
	glm::vec3 barycentric;
	Vertex v;
	for (int x = min.x; x <= max.x; x++) {
		for (int y = min.y; y <= max.y; y++) {
			pos.x = x;
			pos.y = y;
			barycentric = getBarycentric(v1, v2, v3, pos); // 计算重心坐标
			if (barycentric[0] >= 0 && barycentric[1] >= 0 && barycentric[2] >= 0) { // 点在三角形内部
				v = Lerp(v1, v2, v3, barycentric);
				frameBuffer->drawPixel(v.windowPos.x, v.windowPos.y, shader->fragmentShader(v));
			}
		}
	}
}

/*
* 光栅化三角形
* 使用普通平面坐标系, 将三角形拆分成上下两部分分别使用扫描线算法进行填充与插值
*/
void Pipline::drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) {

	// 将v1,v2,v3按照纵坐标由小到大排序
	Vertex arr[3] = { v1, v2, v3 };
	if (arr[0].windowPos.y > arr[1].windowPos.y) std::swap(arr[0], arr[1]);
	if (arr[1].windowPos.y > arr[2].windowPos.y) std::swap(arr[1], arr[2]);
	if (arr[0].windowPos.y > arr[1].windowPos.y) std::swap(arr[0], arr[1]);

	// 填充前初始化
	int minY = arr[0].windowPos.y, midY = arr[1].windowPos.y, maxY = arr[2].windowPos.y;
	int bottomHeight = midY - minY, upperHeight = maxY - midY, totalHeight = maxY - minY;

	bool flag = arr[1].windowPos.x < arr[2].windowPos.x;
	Vertex leftPoint = flag ? arr[1] : Lerp(arr[0], arr[2], float(bottomHeight) / totalHeight);
	Vertex rightPoint = flag ? Lerp(arr[0], arr[2], float(bottomHeight) / totalHeight) : arr[1];
	Vertex leftpos, rightpos;
	float weight;

	// 填充下半部分
	for (int y = minY; y < midY; y++) {
		weight = float(y - minY) / bottomHeight;
		leftpos = Lerp(arr[0], leftPoint, weight);
		rightpos = Lerp(arr[0], rightPoint, weight);
		scanLineFilling(leftpos, rightpos);
	}

	// 填充上半部分
	for (int y = midY; y <= maxY; y++) {
		weight = float(y - midY) / upperHeight;
		leftpos = Lerp(leftPoint, arr[2], weight);
		rightpos = Lerp(rightPoint, arr[2], weight);
		scanLineFilling(leftpos, rightpos);
	}
}

/*
* 扫描线填充
* 填充并插值从left到right的所有点
*/
void Pipline::scanLineFilling(const Vertex& left, const Vertex& right) {
	
	if (left.windowPos.x == right.windowPos.x) {
		frameBuffer->drawPixel(left.windowPos.x, left.windowPos.y, shader->fragmentShader(left));
		return;
	}
	int xmin = left.windowPos.x + 0.5, xmax = right.windowPos.x + 0.5; // 四舍五入
	int lenght = xmax - xmin;
	int y = left.windowPos.y + 0.5;
	Vertex v;
	for (int x = xmin; x <= xmax; x++) {
		v = Lerp(left, right, float(x - xmin) / lenght);
		frameBuffer->drawPixel(x, y, shader->fragmentShader(v));
	}
}

void Pipline::drawMesh(const Mesh& mesh) {
	int size = mesh.EBO.size();
	if (size == 0) {
		return;
	}
	RawVertex A, B, C;
	Vertex v1, v2, v3;
	for (int i = 0; i < size; i += 3) {
		A = mesh.VBO[mesh.EBO[i]];
		B = mesh.VBO[mesh.EBO[i + 1]];
		C = mesh.VBO[mesh.EBO[i + 2]];

		v1 = shader->vertexShader(A);
		v2 = shader->vertexShader(B);
		v3 = shader->vertexShader(C);

		drawTriangle(v1, v2, v3);
	}
}

#endif // !__WINDOW_H__

