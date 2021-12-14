#ifndef __MATH_H__
#define __MATH_H__

#include <glm/glm.hpp>
#include <vector>

class Vertex;

/*
* 计算三角形重心坐标
*/
glm::vec3 getBarycentric(const Vertex& A, const Vertex& B, const Vertex& C, const glm::vec2& P);

/*
* 计算窗口矩阵
* glm使用行矩阵, 存储时需要转置
* Vp = [ w/2,  0 ,  0 , w/2+x,
*		  0 , h/2,  0 , h/2+y,
*		  0 ,  0 ,  1 ,   0  ,
*		  0 ,  0 ,  0 ,   1  ]
*/
glm::mat4 getViewPortMatrix(int x, int y, int width, int height);

/*
* 计算摄像机矩阵
* glm使用行矩阵, 存储时需要转置
*/
glm::mat4 getViewMatrix(const glm::vec3& pos, const glm::vec3 front, const glm::vec3 right, const glm::vec3 up);

/*
* 计算投影透视矩阵
* glm使用行矩阵, 存储时需要转置
*/
glm::mat4 getPerspectiveMatrix(const float& fovy, const float& aspect, const float& n, const float& f);

/*
* 透视除法
* 将齐次坐标转换为三维坐标
*/
void perspectiveDivision(Vertex& v);

/*
* 背面剔除
*/
bool backFaceCutting(const Vertex& A, const Vertex& B, const Vertex& C);

/*
* 计算视锥平面
* vp = projectMatrix * viewMatrix
*/
void updateViewFrustumPlanes(std::vector<glm::vec4>& planes, const glm::mat4& vp);

/*
* 视锥去除
*/
bool viewFrustumCutting(const Vertex& A, const Vertex& B, const Vertex& C, std::vector<glm::vec4>& planes);


/*
* 线性插值
*/
glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, const float& factor);

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec3& factor);

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, const float& factor);

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& factor);

glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, const float& factor);

glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec3& factor);

float Lerp(const float& f1, const float& f2, const float& factor);

float Lerp(const float& f1, const float& f2, const float& f3, const glm::vec3& factor);

// 两个点之间线性插值
Vertex Lerp(const Vertex& v1, const Vertex& v2, const float& factor);

// 三角形重心坐标系线性插值
Vertex Lerp(const Vertex& v1, const Vertex& v2, const Vertex& v3, const glm::vec3& factor);

// 计算光线反射方向
glm::vec3 reflect(const glm::vec3& lightDir, const glm::vec3& normal);

/*
* 工具函数
*/
float triMin(float a, float b, float c);

float triMax(float a, float b, float c);

/*
* 点到面距离
* 平面方程 Ax + By + Cz + D = 0
* distance = Ax + By + Cz + D
*/
float distance(const glm::vec3& point, const glm::vec4& plane);


#endif // !__MATH_H__

