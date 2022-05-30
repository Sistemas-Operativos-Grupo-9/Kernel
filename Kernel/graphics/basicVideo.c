
#include "graphics/basicVideo.h"
#include "semaphore.h"
#include <stdbool.h>

mode_info_block *const infoBlock = (mode_info_block *)0x0000000000005C00;

static uint32_t WIDTH;
static uint32_t HEIGHT;

void initVideo() {
	WIDTH = infoBlock->x_res;
	HEIGHT = infoBlock->y_res;
}

uint32_t getWidth() { return WIDTH; }
uint32_t getHeight() { return HEIGHT; }
Color colorLerp(Color a, Color b, uint8_t lerp) {
	return (Color){.red = a.red + (b.red - a.red) * lerp / 255,
	               .green = a.green + (b.green - a.green) * lerp / 255,
	               .blue = a.blue + (b.blue - a.blue) * lerp / 255};
}

static void setPixel(Color color, uint16_t x, uint16_t y) {
	((Color *)(uint64_t)infoBlock->physbase)[x + y * WIDTH] = color;
}

uint16_t charOffsetX, charOffsetY;
void setCharOffset(uint16_t widthCount, uint16_t heightCount) {
	charOffsetX = (WIDTH - widthCount * FONT_WIDTH * FONT_SCALE) / 2;
	charOffsetY = (HEIGHT - heightCount * FONT_HEIGHT * FONT_SCALE) / 2;
}

void drawCharAtFree(void(setPixel)(Color, uint16_t, uint16_t), char ch,
                    uint16_t x, uint16_t y, Color background,
                    Color foreground) {
	FONT_ROW_TYPE *l =
	    source_code_pro__regular_letters[source_code_pro__regular_mapping[(
	        uint8_t)ch]];
	for (int fontY = 0; fontY < FONT_HEIGHT; fontY++) {
		for (int fontX = 0; fontX < FONT_WIDTH; fontX++) {
			int val =
			    (l[fontY] >> ((FONT_WIDTH - fontX - 1) * FONT_BPP)) & 0b11;
			Color color = colorLerp(background, foreground, val * (255 / 0b11));
			// int val = (l[fontY] >> ((FONT_WIDTH - fontX - 1) * FONT_BPP)) &
			// 0b1; colorLerp(background, foreground, &color, val * (255 /
			// 0b1));

			for (int sy = 0; sy < FONT_SCALE; sy++) {
				for (int sx = 0; sx < FONT_SCALE; sx++) {
					setPixel(color, x + (fontX * FONT_SCALE + sx),
					         y + (fontY * FONT_SCALE + sy));
				}
			}
		}
	}
}

void drawCharAt(char ch, uint8_t x, uint8_t y, Color background,
                Color foreground) {
	drawCharAtFree(setPixel, ch, charOffsetX + x * (FONT_WIDTH * FONT_SCALE),
	               charOffsetY + y * (FONT_HEIGHT * FONT_SCALE), background,
	               foreground);
}

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

#define drawByPixel(xStart, yStart, xEnd, yEnd, code)                          \
	for (int y = yStart; y < yEnd; y++) {                                      \
		for (int x = xStart; x < xEnd; x++) {                                  \
			code                                                               \
		}                                                                      \
	}

void drawImage(Color (*getPixelColor)(uint16_t x, uint16_t y)) {
	drawByPixel(0, 0, WIDTH, HEIGHT, setPixel(getPixelColor(x, y), x, y);)
}

void drawCircleRaw(uint64_t centerX, uint64_t centerY, uint64_t radius,
                   Color color, uint64_t clipX, uint64_t clipY, uint64_t clipW,
                   uint64_t clipH) {
	drawByPixel(
	    max(clipX, centerX - radius), max(clipY, centerY - radius),
	    min(clipX + clipW, centerX + radius),
	    min(clipY + clipH, centerY + radius),
	    if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <
	        radius * radius) setPixel(color, x, y);)
}

void drawRectangleRaw(uint64_t xStart, uint64_t yStart, uint64_t width,
                      uint64_t height, Color color, uint64_t clipX,
                      uint64_t clipY, uint64_t clipW, uint64_t clipH) {
	drawByPixel(max(clipX, xStart), max(clipY, yStart),
	            min(clipX + clipW, xStart + width),
	            min(clipY + clipH, yStart + height), setPixel(color, x, y);)
}
void drawBitmapRaw(uint64_t xStart, uint64_t yStart, uint64_t width,
                   uint64_t height, Color bitmap[][width], uint64_t clipX,
                   uint64_t clipY, uint64_t clipW, uint64_t clipH) {
	drawByPixel(max(clipX, xStart), max(clipY, yStart),
	            min(clipX + clipW, xStart + width),
	            min(clipY + clipH, yStart + height),
	            setPixel(bitmap[y - yStart][x - xStart], x, y);)
}

uint16_t getOffsetX() { return charOffsetX; }
uint16_t getOffsetY() { return charOffsetY; }

void drawRectangleBorders(uint64_t xStart, uint64_t yStart, uint64_t width,
                          uint64_t height, uint16_t borderSize, Color color) {
	// Top and bottom
	drawRectangleRaw(xStart - borderSize, yStart - borderSize,
	                 width + borderSize * 2, borderSize, color, 0, 0, WIDTH,
	                 HEIGHT);
	drawRectangleRaw(xStart - borderSize, yStart + height,
	                 width + borderSize * 2, borderSize, color, 0, 0, WIDTH,
	                 HEIGHT);

	// Left and right
	drawRectangleRaw(xStart - borderSize, yStart, borderSize, height, color, 0,
	                 0, WIDTH, HEIGHT);
	drawRectangleRaw(xStart + width, yStart, borderSize, height, color, 0, 0,
	                 WIDTH, HEIGHT);
}

#undef drawByPixel
