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
	LINE, MESH
};

class Scene {
public:
	Camera* camera;
	std::vector<Light*> lights;
	std::vector<Object*> objects;
	glm::vec4 backgroundColor;

	Material* currentMaterial;

	glm::mat4 viewMatrix; //  ”Õºæÿ’Û
	glm::mat4 projectMatrix; // Õ∂”∞æÿ’Û
	glm::mat4 viewPortMatrix; // ¥∞ø⁄æÿ’Û

	int width;
	int height;
	FrameBuffer* frameBuffer;
	RenderMode mode;

	Scene(const int& w, const int& h, RenderMode m = RenderMode::MESH);
	~Scene();

	void init();
	void add(Camera* cam);
	void add(Light* light);
	void add(Object* object);
	void setBackgroundColor(const glm::vec4& color);

	void resize(const int& w, const int& h);
	void fillColorBuffer(const glm::vec4& color);
	void display();

	void render();

	void drawMesh(const Mesh* mesh, Shader* shader);
	void drawObject(const Object* obj);

	void writeVertex(Vertex& v, Shader* shader);
	void drawLine(const Vertex& v1, const Vertex& v2);
	void drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, Shader* shader);
	void scanLineFilling(const Vertex& left, const Vertex& right, Shader* shader);
	void drawTriangleByBarycentric(const Vertex& v1, const Vertex& v2, const Vertex& v3, Shader* shader);
};

#endif // !__SCENE_H__

