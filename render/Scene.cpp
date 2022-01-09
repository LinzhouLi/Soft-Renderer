#include "Scene.h"

#include "Math.h"
#include "Shader.h"
#include "Mesh.h"
#include "Object.h"
#include "Light.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Material.h"
#include "Light.h"

Scene::Scene(const int& w, const int& h, RenderMode m) {
	width = w;
	height = h;
	mode = m;
	frameBuffer = nullptr;
}

Scene::~Scene() {
	if (frameBuffer) {
		delete frameBuffer;
	}
	lights.clear();
	std::vector <Light*>().swap(lights); // ����ڴ�
	objects.clear();
	std::vector <Object*>().swap(objects); // ����ڴ�
	frameBuffer = nullptr;
}

void Scene::init() {
	camera = nullptr;
	currentMaterial = nullptr;
	glm::vec4 backgroundColor = glm::vec4(0, 0, 0, 255);
	viewFrustumPlanes.resize(6, glm::vec4(0.0f));

	lights.clear();
	std::vector <Light*>().swap(lights); // ����ڴ�
	objects.clear();
	std::vector <Object*>().swap(objects); // ����ڴ�

	if (frameBuffer) {
		delete frameBuffer;
	}
	frameBuffer = new FrameBuffer(width, height);

	viewPortMatrix = getViewPortMatrix(0, 0, width, height);
	viewMatrix = glm::mat4(1.0f);
	projectMatrix = glm::mat4(1.0f);
}

void Scene::add(Camera* cam) {
	if (camera) {
		delete camera;
	}
	camera = cam;
	updateCamera();
	glm::vec4 temp = projectMatrix * viewMatrix * viewFrustumPlanes[4];
	nearZ = (temp.z / temp.w + 1.0) * 0.5;
	temp = projectMatrix * viewMatrix * viewFrustumPlanes[5];
	farZ = (temp.z / temp.w + 1.0) * 0.5;
}

void Scene::add(Light* light) {
	lights.push_back(light);
}

void Scene::add(Object* object) {
	objects.push_back(object);
	object->material->shader->setScene(this);
}

void Scene::updateCamera() {
	camera->updateAspact(width, height);
	viewMatrix = camera->viewMatrix(); // ���¹۲�ռ����
	projectMatrix = camera->perspectiveMatrix(); // ����ͶӰ����
	updateViewFrustumPlanes(viewFrustumPlanes, projectMatrix * viewMatrix); // ������׶ƽ��
}

void Scene::setBackgroundColor(const glm::vec4& color) {
	backgroundColor = color;
}

void Scene::resize(const int& w, const int& h) {
	width = w;
	height = h;
	frameBuffer->resize(w, h);
	viewPortMatrix = getViewPortMatrix(0, 0, w, h);

	updateCamera();
}

void Scene::fillColorBuffer(const glm::vec4& color) {
	frameBuffer->fillColorBuffer(color);
}

void Scene::display() {
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer->colorBuffer.data());
}

void Scene::writeVertex(Vertex& v, Shader* shader) {
	frameBuffer->drawPixel(v.windowPos.x, v.windowPos.y, shader->fragmentShader(v));
	frameBuffer->setDepth(v.windowPos.x, v.windowPos.y, v.windowPos.z);
}

/*
* ��դ��������
* ʹ����������ϵ, ���������ΰ�Χ�����
* ���㷨���ڲ�ֵ, �������ٶȹ���, ����ʹ��
*/
void Scene::drawTriangleByBarycentric(const Vertex& v1, const Vertex& v2, const Vertex& v3, Shader* shader) {

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
void Scene::drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, Shader* shader) {

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
		scanLineFilling(leftpos, rightpos, shader);
	}

	// ����ϰ벿��
	for (int y = midY; y <= maxY; y++) {
		weight = float(y - midY) / upperHeight;
		leftpos = Lerp(leftPoint, arr[2], weight);
		rightpos = Lerp(rightPoint, arr[2], weight);
		scanLineFilling(leftpos, rightpos, shader);
	}
}

/*
* ɨ�������
* ��䲢��ֵ��left��right�����е�
*/
void Scene::scanLineFilling(const Vertex& left, const Vertex& right, Shader* shader) {

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
		if (v.windowPos.z < depth) { // ��Ȳ���

			// ͸��ӳ��
			v.worldPos /= v.z;
			v.texCoord /= v.z;
			v.normal /= v.z;
			v.color /= v.z;

			// Ƭ����ɫ��
			frameBuffer->drawPixel(x, y, shader->fragmentShader(v));
			frameBuffer->setDepth(x, y, v.windowPos.z);
		}
	}
}

/*
* ����Mesh
* һ��Mesh�ɶ�������湹��
*/
void Scene::drawMesh(const Mesh* mesh, Shader* shader) {
	int size = mesh->EBO.size();
	if (size == 0) {
		return;
	}
	RawVertex A, B, C;
	Vertex v1, v2, v3;
	for (int i = 0; i < size; i += 3) {
		A = *(mesh->VBO.data() + *(mesh->EBO.data() + i));
		B = *(mesh->VBO.data() + *(mesh->EBO.data() + i + 1));
		C = *(mesh->VBO.data() + *(mesh->EBO.data() + i + 2));

		// ������ɫ��
		v1 = shader->vertexShader(A);
		v2 = shader->vertexShader(B);
		v3 = shader->vertexShader(C);

		// ��׶�޳�
		if (viewFrustumCutting(v1, v2, v3, viewFrustumPlanes)) continue;

		// ͸�ӳ���
		perspectiveDivision(v1);
		perspectiveDivision(v2);
		perspectiveDivision(v3);

		// �����޳�
		if (backFaceCutting(v1, v2, v3)) continue;

		// NDC����ת��Ϊ������Ļ����
		v1.windowPos = viewPortMatrix * v1.windowPos;
		v2.windowPos = viewPortMatrix * v2.windowPos;
		v3.windowPos = viewPortMatrix * v3.windowPos;

		if (mode == RenderMode::MESH) {
			drawTriangle(v1, v2, v3, shader);
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
void Scene::drawLine(const Vertex& v1, const Vertex& v2) {
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

void Scene::drawObject(const Object* obj) {
	if (obj->mesh->EBO.empty()) {
		return;
	}
	drawMesh(obj->mesh, obj->material->shader);
}

void Scene::render() {
	updateCamera();
	fillColorBuffer(backgroundColor);
	for (auto object : objects) {
		currentMaterial = object->material;
		drawObject(object);
	}
	display();
}