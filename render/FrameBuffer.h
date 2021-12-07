#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <vector>
#include <glm/glm.hpp>

class FrameBuffer {
private:
	int width, height;
	std::vector<float> depthBuffer; // Éî¶È»º³åÇø

public:
	std::vector<unsigned char> colorBuffer;

	FrameBuffer(const int& w, const int& h);
	~FrameBuffer() = default;

	void resize(const int& w, const int& h);
	void fillColorBuffer(const glm::vec4& color);
	void drawPixel(const int& x, const int& y, const glm::vec4& color);

	float getDepth(const int& x, const int& y);
	void setDepth(const int& x, const int& y, const float& depth);
};

#endif // !__FRAMEBUFFER_H__

