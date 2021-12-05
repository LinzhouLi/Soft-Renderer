#include "tgaimage.h"

/*
*  Bresenham 画线算法
*/
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false; // 是否交换x,y轴
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // 如果斜率大于1, 则交换x,y轴
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	int x;
	for (x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += y1 > y0 ? 1 : -1;
			error2 -= dx * 2;
		}
	}
}

/*
int main() {
	TGAImage image(100, 100, TGAImage::RGB);
	const TGAColor white = TGAColor(255, 255, 255, 255);
	line(60, 70, 20, 10, image, white);
	image.write_tga_file("output.tga");
	return 0;
}*/