#include "Math.h"

#include <algorithm>
#include "Mesh.h"

/*
* 线性插值
*/
glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, const float& factor) {
	return (1 - factor) * v1 + factor * v2;
}

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec3& factor) {
	return factor[0] * v1 + factor[1] * v2 + factor[2] * v3;
}

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, const float& factor) {
	return (1 - factor) * v1 + factor * v2;
}

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& factor) {
	return factor[0] * v1 + factor[1] * v2 + factor[2] * v3;
}

glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, const float& factor) {
	return (1 - factor) * v1 + factor * v2;
}

glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec3& factor) {
	return factor[0] * v1 + factor[1] * v2 + factor[2] * v3;
}

float Lerp(const float& f1, const float& f2, const float& factor) {
	return (1 - factor) * f1 + factor * f2;
}

float Lerp(const float& f1, const float& f2, const float& f3, const glm::vec3& factor) {
	return factor[0] * f1 + factor[1] * f2 + factor[2] * f3;
}

// 两个点之间线性插值
Vertex Lerp(const Vertex& v1, const Vertex& v2, const float& factor) {
	Vertex result;
	result.worldPos = Lerp(v1.worldPos, v2.worldPos, factor);
	result.windowPos = Lerp(v1.windowPos, v2.windowPos, factor);
	result.color = Lerp(v1.color, v2.color, factor);
	result.texCoord = Lerp(v1.texCoord, v2.texCoord, factor);
	result.normal = Lerp(v1.normal, v2.normal, factor);
	result.z = Lerp(v1.z, v2.z, factor);
	return result;
}

// 三角形重心坐标系线性插值
Vertex Lerp(const Vertex& v1, const Vertex& v2, const Vertex& v3, const glm::vec3& factor) {
	Vertex result;
	result.worldPos = Lerp(v1.worldPos, v2.worldPos, v3.worldPos, factor);
	result.windowPos = Lerp(v1.windowPos, v2.windowPos, v3.windowPos, factor);
	result.color = Lerp(v1.color, v2.color, v3.color, factor);
	result.texCoord = Lerp(v1.texCoord, v2.texCoord, v3.texCoord, factor);
	result.normal = Lerp(v1.normal, v2.normal, v3.normal, factor);
	return result;
}

/*
* 计算三角形重心坐标
*/
glm::vec3 getBarycentric(const Vertex& A, const Vertex& B, const Vertex& C, const glm::vec2& P) {
	glm::vec3 result;
	glm::vec2 AB(B.windowPos.x - A.windowPos.x, B.windowPos.y - A.windowPos.y);
	glm::vec2 AC(C.windowPos.x - A.windowPos.x, C.windowPos.y - A.windowPos.y);
	glm::vec2 AP(P.x - A.windowPos.x, P.y - A.windowPos.y);
	float S = AB.x * AC.y - AB.y * AC.x;
	result[1] = (AP.x * AC.y - AP.y * AC.x) / S;
	result[2] = (AB.x * AP.y - AB.y * AP.x) / S;
	result[0] = 1 - result[1] - result[2];
	return result;
}

/*
* 计算窗口矩阵
* glm使用行矩阵, 存储时需要转置
* Vp = [ w/2,  0 ,  0 , w/2+x,
*		  0 , h/2,  0 , h/2+y,
*		  0 ,  0 ,  1 ,   0  ,
*		  0 ,  0 ,  0 ,   1  ]
*/
glm::mat4 getViewPortMatrix(int x, int y, int width, int height) {
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = width / 2.0f;
	result[1][1] = height / 2.0f;
	result[3][0] = width / 2.0f + x;
	result[3][1] = height / 2.0f + y;
	return result;
}

/*
* 计算摄像机矩阵
* glm使用行矩阵, 存储时需要转置
*/
glm::mat4 getViewMatrix(const glm::vec3& pos, const glm::vec3 front, const glm::vec3 right, const glm::vec3 up) {
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = right.x;
	result[1][0] = right.y;
	result[2][0] = right.z;
	result[3][0] = -glm::dot(right, pos);
	result[0][1] = up.x;
	result[1][1] = up.y;
	result[2][1] = up.z;
	result[3][1] = -glm::dot(up, pos);
	result[0][2] = -front.x;
	result[1][2] = -front.y;
	result[2][2] = -front.z;
	result[3][2] = glm::dot(front, pos);
	return result;
}

/*
* 计算投影透视矩阵
* glm使用行矩阵, 存储时需要转置
*/
glm::mat4 getPerspectiveMatrix(const float& fovy, const float& aspect, const float& n, const float& f) {
	glm::mat4 result = glm::mat4(0.0f);
	float tanHalfFov = tan(fovy * 0.5f);
	result[0][0] = 1.0f / (aspect * tanHalfFov);
	result[1][1] = 1.0f / (tanHalfFov);
	result[2][2] = -(f + n) / (f - n);
	result[2][3] = -1.0f;
	result[3][2] = (-2.0f * n * f) / (f - n);
	return result;
}

/*
* 透视除法
* 将齐次坐标转换为三维坐标
*/
void perspectiveDivision(Vertex& v) {
	v.windowPos /= v.windowPos.w;
	v.windowPos.w = 1.0f;
	v.windowPos.z = (v.windowPos.z + 1.0) * 0.5;
}

/*
* 背面剔除
*/
bool backFaceCutting(const Vertex& A, const Vertex& B, const Vertex& C) {
	glm::vec3 AB = glm::vec3(B.windowPos.x - A.windowPos.x, B.windowPos.y - A.windowPos.y, B.windowPos.z - A.windowPos.z);
	glm::vec3 AC = glm::vec3(C.windowPos.x - A.windowPos.x, C.windowPos.y - A.windowPos.y, C.windowPos.z - A.windowPos.z);
	glm::vec3 normal = glm::cross(AB, AC); // 面的法向量
	glm::vec3 view = glm::vec3(0, 0, 1);
	return glm::dot(normal, view) < 0;
}

/*
* 计算视锥平面
*/
void updateViewFrustumPlanes(std::vector<glm::vec4>& planes, const glm::mat4& vp) {
	glm::vec4* p = planes.data();
	//左侧  
	p->x = vp[0][3] + vp[0][0];
	p->y = vp[1][3] + vp[1][0];
	p->z = vp[2][3] + vp[2][0];
	p->w = vp[3][3] + vp[3][0];
	//右侧
	(p + 1)->x = vp[0][3] - vp[0][0];
	(p + 1)->y = vp[1][3] - vp[1][0];
	(p + 1)->z = vp[2][3] - vp[2][0];
	(p + 1)->w = vp[3][3] - vp[3][0];
	//上侧
	(p + 2)->x = vp[0][3] - vp[0][1];
	(p + 2)->y = vp[1][3] - vp[1][1];
	(p + 2)->z = vp[2][3] - vp[2][1];
	(p + 2)->w = vp[3][3] - vp[3][1];
	//下侧
	(p + 3)->x = vp[0][3] + vp[0][1];
	(p + 3)->y = vp[1][3] + vp[1][1];
	(p + 3)->z = vp[2][3] + vp[2][1];
	(p + 3)->w = vp[3][3] + vp[3][1];
	//Near
	(p + 4)->x = vp[0][3] + vp[0][2];
	(p + 4)->y = vp[1][3] + vp[1][2];
	(p + 4)->z = vp[2][3] + vp[2][2];
	(p + 4)->w = vp[3][3] + vp[3][2];
	//Far
	(p + 5)->x = vp[0][3] - vp[0][2];
	(p + 5)->y = vp[1][3] - vp[1][2];
	(p + 5)->z = vp[2][3] - vp[2][2];
	(p + 5)->w = vp[3][3] - vp[3][2];
}

/*
* 视锥去除
*/
bool viewFrustumCutting(const Vertex& A, const Vertex& B, const Vertex& C, std::vector<glm::vec4>& planes) {
	glm::vec3 minPoint, maxPoint;
	glm::vec4* p = planes.data();
	minPoint.x = triMin(A.worldPos.x, B.worldPos.x, C.worldPos.x);
	minPoint.y = triMin(A.worldPos.y, B.worldPos.y, C.worldPos.y);
	minPoint.z = triMin(A.worldPos.z, B.worldPos.z, C.worldPos.z);
	maxPoint.x = triMax(A.worldPos.x, B.worldPos.x, C.worldPos.x);
	maxPoint.y = triMax(A.worldPos.y, B.worldPos.y, C.worldPos.y);
	maxPoint.z = triMax(A.worldPos.z, B.worldPos.z, C.worldPos.z);
	
	// near far
	if (distance(minPoint, *(p + 4)) < 0 || distance(maxPoint, *(p + 4)) < 0) return true;
	if (distance(minPoint, *(p + 5)) < 0 || distance(maxPoint, *(p + 5)) < 0) return true;

	// other plans
	for (int i = 0; i < 4; i++) {
		if (distance(minPoint, *(p + i)) < 0 || distance(maxPoint, *(p + i)) < 0) return true;
	}
	return false;
}

// 计算光线反射方向
glm::vec3 reflect(const glm::vec3& lightDir, const glm::vec3& normal) {
	return glm::normalize(lightDir - 2 * glm::dot(normal, lightDir) * normal);
}


/*
* 工具函数
*/
float triMin(float a, float b, float c) {
	float result = std::min(a, b);
	result = std::min(result, c);
	return result;
}

float triMax(float a, float b, float c) {
	float result = std::max(a, b);
	result = std::max(result, c);
	return result;
}

/*
* 点到面距离
* 平面方程 Ax + By + Cz + D = 0
* distance = Ax + By + Cz + D
*/
float distance(const glm::vec3& point, const glm::vec4& plane) {
	return point.x * plane.x + point.y * plane.y + point.z * plane.z + plane.w;
}