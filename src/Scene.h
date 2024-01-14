#ifndef __SCENE_H__
#define __SCENE_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

class Vertex;
class Mesh;
class Shader;
class Vertex;
class Object;
class Light;
class Camera;
class FrameBuffer;
class Material;

enum class RenderMode {
	LINE, MESH_SCANLINE, MESH_BOUNDARY
};

class Scene {
public:
	Camera* camera;
	std::vector<Light*> lights;
	std::vector<Object*> objects;
	std::vector<glm::vec4> viewFrustumPlanes;
	glm::vec4 backgroundColor;

	float nearZ;
	float farZ;

	Material* currentMaterial;

	glm::mat4 viewMatrix; // 观察空间矩阵
	glm::mat4 projectMatrix; // 投影矩阵
	glm::mat4 viewPortMatrix; // 窗口矩阵

	int width;
	int height;
	FrameBuffer* frameBuffer;
	RenderMode mode;

	Scene(const int& w, const int& h, RenderMode m = RenderMode::MESH_SCANLINE);
	~Scene();

	void init();
	void add(Camera* cam);
	void add(Light* light);
	void add(Object* object);
	void updateCamera();
	void setBackgroundColor(const glm::vec4& color);

	void resize(const int& w, const int& h);
	void fillColorBuffer(const glm::vec4& color);
	void display();

	void render();

	void drawMesh(const Mesh* mesh, Shader* shader);
	void drawObject(const Object* obj);

	void drawVertex(int x, int y, Vertex& v, Shader* shader);
	void drawLine(const Vertex& v1, const Vertex& v2);
	void drawTriangleScanLine(const Vertex& v1, const Vertex& v2, const Vertex& v3, Shader* shader);
	void scanLineFilling(const Vertex& left, const Vertex& right, Shader* shader);
	void drawTriangleBoundary(const Vertex& v1, const Vertex& v2, const Vertex& v3, Shader* shader);
};

#endif // !__SCENE_H__

