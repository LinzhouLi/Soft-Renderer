#ifndef __MATH_H__
#define __MATH_H__

#include <glm/glm.hpp>
#include <vector>

class Vertex;

/*
* ������������������
*/
glm::vec3 getBarycentric(const Vertex& A, const Vertex& B, const Vertex& C, const glm::vec2& P);

/*
* ���㴰�ھ���
* glmʹ���о���, �洢ʱ��Ҫת��
* Vp = [ w/2,  0 ,  0 , w/2+x,
*		  0 , h/2,  0 , h/2+y,
*		  0 ,  0 ,  1 ,   0  ,
*		  0 ,  0 ,  0 ,   1  ]
*/
glm::mat4 getViewPortMatrix(int x, int y, int width, int height);

/*
* �������������
* glmʹ���о���, �洢ʱ��Ҫת��
*/
glm::mat4 getViewMatrix(const glm::vec3& pos, const glm::vec3 front, const glm::vec3 right, const glm::vec3 up);

/*
* ����ͶӰ͸�Ӿ���
* glmʹ���о���, �洢ʱ��Ҫת��
*/
glm::mat4 getPerspectiveMatrix(const float& fovy, const float& aspect, const float& n, const float& f);

/*
* ͸�ӳ���
* ���������ת��Ϊ��ά����
*/
void perspectiveDivision(Vertex& v);

/*
* �����޳�
*/
bool backFaceCutting(const Vertex& A, const Vertex& B, const Vertex& C);

/*
* ������׶ƽ��
* vp = projectMatrix * viewMatrix
*/
void updateViewFrustumPlanes(std::vector<glm::vec4>& planes, const glm::mat4& vp);

/*
* ��׶ȥ��
*/
bool viewFrustumCutting(const Vertex& A, const Vertex& B, const Vertex& C, std::vector<glm::vec4>& planes);


/*
* ���Բ�ֵ
*/
glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, const float& factor);

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec3& factor);

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, const float& factor);

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& factor);

glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, const float& factor);

glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec3& factor);

float Lerp(const float& f1, const float& f2, const float& factor);

float Lerp(const float& f1, const float& f2, const float& f3, const glm::vec3& factor);

// ������֮�����Բ�ֵ
Vertex Lerp(const Vertex& v1, const Vertex& v2, const float& factor);

// ��������������ϵ���Բ�ֵ
Vertex Lerp(const Vertex& v1, const Vertex& v2, const Vertex& v3, const glm::vec3& factor);

// ������߷��䷽��
glm::vec3 reflect(const glm::vec3& lightDir, const glm::vec3& normal);

/*
* ���ߺ���
*/
float triMin(float a, float b, float c);

float triMax(float a, float b, float c);

/*
* �㵽�����
* ƽ�淽�� Ax + By + Cz + D = 0
* distance = Ax + By + Cz + D
*/
float distance(const glm::vec3& point, const glm::vec4& plane);


#endif // !__MATH_H__

