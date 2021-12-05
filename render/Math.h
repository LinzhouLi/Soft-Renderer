#ifndef __MATH_H__
#define __MATH_H__

#include "Vertex.h"
#include <glm/glm.hpp>

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

// 两个点之间线性插值
Vertex Lerp(const Vertex& v1, const Vertex& v2, const float& factor) {
	Vertex result;
	result.worldPos = Lerp(v1.worldPos, v2.worldPos, factor);
	result.windowPos = Lerp(v1.windowPos, v2.windowPos, factor);
	result.color = Lerp(v1.color, v2.color, factor);
	result.texCoord = Lerp(v1.texCoord, v2.texCoord, factor);
	result.normal = Lerp(v1.normal, v2.normal, factor);
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
* 计算视口矩阵
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
* 工具函数
*/
template<class T>
T triMin(T a, T b, T c) {
	T result = std::min(a, b);
	result = std::min(result, c);
	return result;
}

template<class T>
T triMax(T a, T b, T c) {
	T result = std::max(a, b);
	result = std::max(result, c);
	return result;
}

#endif // !__MATH_H__

