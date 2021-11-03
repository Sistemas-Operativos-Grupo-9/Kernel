#include <color.h>
#include <print.h>
#include <stdbool.h>
#include <stdio.h>
#include <syscall.h>
WindowInfo windowInfo;

uint16_t mandelbrot(uint16_t maxIter, double cx, double cy) {
	uint16_t iter = 0;
	double x = cx, y = cy;
	while (iter < maxIter && (x * x + y * y) <= 4) {
		double tempX = x * x - y * y + cx;
		y = 2 * x * y + cy;
		x = tempX;
		iter++;
	}
	return iter;
}

Color colors[] = {GREY_N3, GREY_N2, GREY_N1, GREY, GREY_1, GREY_2, GREY_3};
Color iterToColor(uint16_t maxIter, uint16_t iters) {
	if (iters == maxIter)
		return BLACK;
	return colors[iters % (sizeof(colors) / sizeof(Color))];
}

void render(uint16_t maxIter, double centerX, double centerY, float zoom) {
	setGraphic(false);
	int width = windowInfo.pixelWidth;
	int height = windowInfo.pixelHeight;
	Color buffer[height][width];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			double rx, ry;
			rx = ((double)(x - width / 2) / height * 4) / zoom + centerX;
			ry = ((double)(y - height / 2) / height * 4) / zoom + centerY;
			Color color = iterToColor(maxIter, mandelbrot(maxIter, rx, ry));
			buffer[y][x] = color;
		}
		// printUnsigned(y, 10);
		// printUnsigned(height, 10);
		// putchar('\n');
		if (y % 10 == 0 || y + 1 == height)
			printPercentage((float)(y + 1) / height);
	}

	setGraphic(true);
	drawBitmap(0, 0, width, height, buffer);
}

int main() {
	windowInfo = getWindowInfo();
	float zoom = 1;
	uint16_t maxIter = 10;

	KeyStroke key;
	do {
		render(maxIter, -.743643135, .131825963, zoom);
		key = readKeyStroke(0);
		if (key.isPrintable) {
			switch (key.data) {
			case '.':
				zoom *= 3;
				break;
			case ',':
				zoom /= 3;
				break;
			case 'l':
				maxIter *= 2;
				break;
			case 'k':
				maxIter /= 2;
				break;
			case 'q':
				break;
			}
		}
	} while (key.data != 'q');

	setGraphic(false);
	return 0;
}