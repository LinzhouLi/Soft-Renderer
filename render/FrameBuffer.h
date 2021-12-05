#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"

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

FrameBuffer::FrameBuffer(const int& w, const int& h) {
	width = w;
	height = h;
	colorBuffer.resize(4 * w * h, 0);
	depthBuffer.resize(w * h, 1.0f);
}

void FrameBuffer::resize(const int& w, const int& h) {
	width = w;
	height = h;
	colorBuffer.resize(4 * w * h, 0);
	depthBuffer.resize(w * h, 1.0f);
}

void FrameBuffer::fillColorBuffer(const glm::vec4& color) {
	unsigned char* p = this->colorBuffer.data();
	for (int i = 0; i < width * height * 4; i += 4) {
		*(p + i) = color.r;
		*(p + i + 1) = color.g;
		*(p + i + 2) = color.b;
		*(p + i + 3) = color.a;
	}
	depthBuffer.assign(width * height, 1.0f);
}

void FrameBuffer::drawPixel(const int& x, const int& y, const glm::vec4& color) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return;
	}
	int xy = y * width + x;
	unsigned char* p = this->colorBuffer.data();
	*(p + xy * 4) = color.r;
	*(p + xy * 4 + 1) = color.g;
	*(p + xy * 4 + 2) = color.b;
	*(p + xy * 4 + 3) = color.a;
}

float FrameBuffer::getDepth(const int& x, const int& y) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return 1.0f;
	}
	return *(depthBuffer.data() + y * width + x);
}

void FrameBuffer::setDepth(const int& x, const int& y, const float& depth) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return;
	}
	*(depthBuffer.data() + y * width + x) = depth;
}

#endif // !__FRAMEBUFFER_H__

