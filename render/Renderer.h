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

enum class RenderMode {
	LINE, MESH
};

class Renderer {
private:
	int width;
	int height;
	FrameBuffer* frameBuffer;
	Shader* shader;
	glm::mat4 viewPortMatrix;
	RenderMode mode;

public:
	Renderer(const int& w, const int& h, RenderMode m = RenderMode::MESH)
		: width(w), height(h), frameBuffer(nullptr), shader(nullptr), mode(m) { }
	~Renderer();

	void init();
	void display();

	void resize(const int& w, const int& h);
	void fillColorBuffer(const glm::vec4& color);

	void setModelMatrix(const glm::mat4& m);
	void setViewMatrix(const glm::mat4& m);
	void setProjectMatrix(const glm::mat4& m);

	void drawLine(const Vertex& v1, const Vertex& v2);
	void scanLineFilling(const Vertex& left, const Vertex& right);
	void drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	void drawTriangleByBarycentric(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	void drawMesh(const Mesh& mesh);
};

Renderer::~Renderer() {
	if (frameBuffer) {
		delete frameBuffer;
	}
	if (shader) {
		delete shader;
	}
	frameBuffer = nullptr;
	shader = nullptr;
}

void Renderer::init() {
	if (frameBuffer) {
		delete frameBuffer;
	}
	if (shader) {
		delete shader;
	}
	viewPortMatrix = getViewPortMatrix(0, 0, width, height);
	shader = new Shader;
	frameBuffer = new FrameBuffer(width, height);
}

void Renderer::display() {
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer->colorBuffer.data());
}

void Renderer::resize(const int& w, const int& h) {
	width = w;
	height = h;
	frameBuffer->resize(w, h);
	viewPortMatrix = getViewPortMatrix(0, 0, w, h);
}

void Renderer::fillColorBuffer(const glm::vec4& color) {
	frameBuffer->fillColorBuffer(color);
}

void Renderer::setModelMatrix(const glm::mat4& m) {
	shader->setModelMatrix(m);
}

void Renderer::setViewMatrix(const glm::mat4& m) {
	shader->setViewMatrix(m);
}

void Renderer::setProjectMatrix(const glm::mat4& m) {
	shader->setProjectMatrix(m);
}

/*
* ��դ��������
* ʹ����������ϵ, ���������ΰ�Χ�����
* ���㷨���ڲ�ֵ, �������ٶȹ���, ����ʹ��
*/
void Renderer::drawTriangleByBarycentric(const Vertex& v1, const Vertex& v2, const Vertex& v3) {

	// �����Χ��
	glm::vec2 min, max;
	min.x = triMin(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	min.y = triMin(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	max.x = triMax(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	max.y = triMax(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	// ������Χ���еĵ�
	glm::vec2 pos;
	glm::vec3 barycentric;
	Vertex v;
	for (int x = min.x; x <= max.x; x++) {
		for (int y = min.y; y <= max.y; y++) {
			pos.x = x;
			pos.y = y;
			barycentric = getBarycentric(v1, v2, v3, pos); // ������������
			if (barycentric[0] >= 0 && barycentric[1] >= 0 && barycentric[2] >= 0) { // �����������ڲ�
				v = Lerp(v1, v2, v3, barycentric);
				frameBuffer->drawPixel(v.windowPos.x, v.windowPos.y, shader->fragmentShader(v));
			}
		}
	}
}

/*
* ��դ��������
* ʹ����ͨƽ������ϵ, �������β�ֳ����������ֱַ�ʹ��ɨ�����㷨����������ֵ
*/
void Renderer::drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) {

	// ��v1,v2,v3������������С��������
	Vertex arr[3] = { v1, v2, v3 };
	if (arr[0].windowPos.y > arr[1].windowPos.y) std::swap(arr[0], arr[1]);
	if (arr[1].windowPos.y > arr[2].windowPos.y) std::swap(arr[1], arr[2]);
	if (arr[0].windowPos.y > arr[1].windowPos.y) std::swap(arr[0], arr[1]);

	// ���ǰ��ʼ��
	int minY = arr[0].windowPos.y, midY = arr[1].windowPos.y, maxY = arr[2].windowPos.y;
	int bottomHeight = midY - minY, upperHeight = maxY - midY, totalHeight = maxY - minY;

	Vertex leftPoint = Lerp(arr[0], arr[2], float(bottomHeight) / totalHeight);
	Vertex rightPoint = arr[1];
	if (leftPoint.windowPos.x > rightPoint.windowPos.x) std::swap(leftPoint, rightPoint);
	Vertex leftpos, rightpos;
	float weight;

	// ����°벿��
	for (int y = minY; y < midY; y++) {
		weight = float(y - minY) / bottomHeight;
		leftpos = Lerp(arr[0], leftPoint, weight);
		rightpos = Lerp(arr[0], rightPoint, weight);
		scanLineFilling(leftpos, rightpos);
	}

	// ����ϰ벿��
	for (int y = midY; y <= maxY; y++) {
		weight = float(y - midY) / upperHeight;
		leftpos = Lerp(leftPoint, arr[2], weight);
		rightpos = Lerp(rightPoint, arr[2], weight);
		scanLineFilling(leftpos, rightpos);
	}
}

/*
* ɨ�������
* ��䲢��ֵ��left��right�����е�
*/
void Renderer::scanLineFilling(const Vertex& left, const Vertex& right) {
	
	if (left.windowPos.x == right.windowPos.x) {
		frameBuffer->drawPixel(left.windowPos.x, left.windowPos.y, shader->fragmentShader(left));
		return;
	}

	int xmin = left.windowPos.x, xmax = right.windowPos.x; // 
	int lenght = xmax - xmin;
	int y = left.windowPos.y;
	Vertex v;
	float depth;

	for (int x = xmin; x <= xmax; x++) {
		v = Lerp(left, right, float(x - xmin) / lenght);

		depth = frameBuffer->getDepth(x, y);
		frameBuffer->drawPixel(x, y, shader->fragmentShader(v));
		if (v.windowPos.z < depth) { // ��Ȳ���
			frameBuffer->drawPixel(x, y, shader->fragmentShader(v));
			frameBuffer->setDepth(x, y, v.windowPos.z);
		}
	}
}

/*
* ����Mesh
* һ��Mesh�ɶ�������湹��
*/
void Renderer::drawMesh(const Mesh& mesh) {
	int size = mesh.EBO.size();
	if (size == 0) {
		return;
	}
	RawVertex A, B, C;
	Vertex v1, v2, v3;
	for (int i = 0; i < size; i += 3) {
		A = *(mesh.VBO.data() + *(mesh.EBO.data() + i));
		B = *(mesh.VBO.data() + *(mesh.EBO.data() + i + 1));
		C = *(mesh.VBO.data() + *(mesh.EBO.data() + i + 2));

		// ������ɫ��
		v1 = shader->vertexShader(A);
		v2 = shader->vertexShader(B);
		v3 = shader->vertexShader(C);

		// ͸�ӳ���
		perspectiveDivision(v1);
		perspectiveDivision(v2);
		perspectiveDivision(v3);

		if (!backFaceCutting(v1, v2, v3)) continue;

		v1.windowPos = viewPortMatrix * v1.windowPos;
		v2.windowPos = viewPortMatrix * v2.windowPos;
		v3.windowPos = viewPortMatrix * v3.windowPos;

		if (mode == RenderMode::MESH) {
			drawTriangle(v1, v2, v3);
		}
		else if (mode == RenderMode::LINE) {
			drawLine(v1, v2);
			drawLine(v2, v3);
			drawLine(v1, v3);
		}
	}
}

/*
* Bresenham�����㷨
*/
void Renderer::drawLine(const Vertex& v1, const Vertex& v2) {
	int x0 = v1.windowPos.x, x1 = v2.windowPos.x;
	int y0 = v1.windowPos.y, y1 = v2.windowPos.y;
	bool steep = false; // �Ƿ񽻻�x,y��
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // ���б�ʴ���1, �򽻻�x,y��
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	int x;
	for (x = x0; x <= x1; x++) {
		if (steep) {
			frameBuffer->drawPixel(y, x, glm::vec4(0, 0, 0, 255));
		}
		else {
			frameBuffer->drawPixel(x, y, glm::vec4(0, 0, 0, 255));
		}
		error2 += derror2;
		if (error2 > dx) {
			y += y1 > y0 ? 1 : -1;
			error2 -= dx * 2;
		}
	}
}

#endif // !__WINDOW_H__

