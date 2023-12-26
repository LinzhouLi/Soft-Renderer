#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>
#include <glm/glm.hpp>

class Texture {
public:
	int width;
	int height;
	int nChannels;
	unsigned char* data;

	Texture() = default;
	Texture(const std::string& path);
	~Texture();
	
	void loadImage(const std::string& path);
	glm::vec4 getColor(const int& x, const int& y);
	glm::vec4 sample2D(const glm::vec2& texCoord);
};

#endif // !__TEXTURE_H__
