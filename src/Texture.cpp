#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION // 用于stb_image.h文件
#include <stb_image.h>
#endif // !STB_IMAGE_IMPLEMENTATION

Texture::Texture(const std::string& path) {
	data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
}

Texture::~Texture() {
	if (data) {
		delete data;
	}
}

void Texture::loadImage(const std::string& path) {
	if (data) {
		delete data;
	}
	data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
}

glm::vec4 Texture::getColor(const int& x, const int& y) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return glm::vec4(0, 0, 0, 0);
	}
	int xy = nChannels * (y * width + x);
	return glm::vec4(*(data + xy), *(data + xy + 1), *(data + xy + 2), *(data + xy + 3));
}

glm::vec4 Texture::sample2D(const glm::vec2& texCoord) {
	float x = texCoord.x - floor(texCoord.x);
	float y = texCoord.y - floor(texCoord.y);
	return getColor(x * (width - 1), y * (height - 1));
}