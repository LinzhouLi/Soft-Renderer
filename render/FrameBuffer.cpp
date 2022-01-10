#include "FrameBuffer.h"

#include "Mesh.h"

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
	*(p + xy * 4) = std::min(color.r, 255.0f);
	*(p + xy * 4 + 1) = std::min(color.g, 255.0f);
	*(p + xy * 4 + 2) = std::min(color.b, 255.0f);
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