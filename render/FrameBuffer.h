#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"

class FrameBuffer {
private:
	int width, height;

public:
	std::vector<unsigned char> colorBuffer;

	FrameBuffer(const int& w, const int& h);
	~FrameBuffer() = default;

	void resize(const int& w, const int& h);
	void fillColorBuffer(const glm::vec4& color);
	void drawPixel(const int& x, const int& y, const glm::vec4& color);
};

FrameBuffer::FrameBuffer(const int& w, const int& h) {
	this->width = w;
	this->height = h;
	this->colorBuffer.resize(4 * w * h, 0);
}

void FrameBuffer::resize(const int& w, const int& h) {
	this->width = w;
	this->height = h;
	this->colorBuffer.resize(4 * w * h, 0);
}

void FrameBuffer::fillColorBuffer(const glm::vec4& color) {
	unsigned char* p = this->colorBuffer.data();
	for (int i = 0; i < width * height * 4; i += 4) {
		*(p + i) = color.r;
		*(p + i + 1) = color.g;
		*(p + i + 2) = color.b;
		*(p + i + 3) = color.a;
	}
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

#endif // !__FRAMEBUFFER_H__

