
#include <stdbool.h>
// #include "Fonts/monogram.h"
#include "Fonts/SourceCodePro-Regular.h"
#include "basicVideo.h"

#define FONT_SCALE 1

mode_info_block * const infoBlock = (mode_info_block *)0x0000000000005C00;

static uint32_t WIDTH;
static uint32_t HEIGHT;

void initVideo() {
    WIDTH = infoBlock->x_res;
    HEIGHT = infoBlock->y_res;
}

uint32_t getWidth() {
    return WIDTH;
}
uint32_t getHeight() {
    return HEIGHT;
}
Color colorLerp(Color a, Color b, uint8_t lerp) {
    return (Color) {
        .red = a.red + (b.red - a.red) * lerp / 255,
        .green = a.green + (b.green - a.green) * lerp / 255,
        .blue = a.blue + (b.blue - a.blue) * lerp / 255
    };
}

void setPixel(Color color, int x, int y) {
    ((Color *)(uint64_t)infoBlock->physbase)[x + y * WIDTH] = color;
}

uint16_t charOffsetX, charOffsetY;
void setCharOffset(uint16_t widthCount, uint16_t heightCount) {
    charOffsetX = (WIDTH - widthCount * FONT_WIDTH * FONT_SCALE) / 2;
    charOffsetY = (HEIGHT - heightCount * FONT_HEIGHT * FONT_SCALE) / 2;
}
void drawCharAt(char ch, uint8_t x, uint8_t y, Color background, Color foreground) {
    FONT_ROW_TYPE *l = font_letters[font_mapping[(uint8_t)ch]];
    for (int fontY = 0; fontY < FONT_HEIGHT; fontY++) {
        for (int fontX = 0; fontX < FONT_WIDTH; fontX++) {
            int val = (l[fontY] >> ((FONT_WIDTH - fontX - 1) * FONT_BPP)) & 0b11;
            Color color = colorLerp(background, foreground, val * (255 / 0b11));
            // int val = (l[fontY] >> ((FONT_WIDTH - fontX - 1) * FONT_BPP)) & 0b1;
            // colorLerp(background, foreground, &color, val * (255 / 0b1));
            
            for (int sy = 0; sy < FONT_SCALE; sy++) {
                for (int sx = 0; sx < FONT_SCALE; sx++) {
                    setPixel(color, charOffsetX + x * (FONT_WIDTH * FONT_SCALE) + (fontX * FONT_SCALE + sx), charOffsetY + y * (FONT_HEIGHT * FONT_SCALE) + (fontY * FONT_SCALE + sy));
                }
            }
        }
    }
}

#define drawByPixel(xStart, yStart, xEnd, yEnd, code)           \
    for (int y = yStart; y < yEnd; y++) {                       \
        for (int x = xStart; x < xEnd; x++) {                   \
            code                                                \
        }                                                       \
    }


void drawImage(Color (*getPixelColor)(uint64_t x, uint64_t y)) {
    drawByPixel(0, 0, WIDTH, HEIGHT,
        setPixel(getPixelColor(x, y), x, y);
    )
}

void drawCircle(uint64_t centerX, uint64_t centerY, uint64_t radius, Color color) {
    drawByPixel(centerX - radius, centerY - radius, centerX + radius, centerY + radius,
        if ((x * centerX) * (x * centerX) + (y * centerY) * (y * centerY) < radius * radius)
            setPixel(color, x, y);
    )
}

void drawRectangle(uint64_t xStart, uint64_t yStart, uint64_t width, uint64_t height, Color color) {
    drawByPixel(xStart, yStart, xStart + width, yStart + height,
        setPixel(color, x, y);
    )
}

uint16_t getOffsetX() {
    return charOffsetX;
}
uint16_t getOffsetY() {
    return charOffsetY;
}

uint16_t getFontWidth() {
    return FONT_WIDTH * FONT_SCALE;
}

uint16_t getFontHeight() {
    return FONT_HEIGHT * FONT_SCALE;
}

void drawRectangleBorders(uint64_t xStart, uint64_t yStart, uint64_t width, uint64_t height, uint16_t borderSize, Color color) {
    // Top and bottom
    drawRectangle(xStart - borderSize, yStart - borderSize, width + borderSize * 2, borderSize, color);
    drawRectangle(xStart - borderSize, yStart + height, width + borderSize * 2, borderSize, color);

    // Left and right
    drawRectangle(xStart - borderSize, yStart, borderSize, height, color);
    drawRectangle(xStart + width, yStart, borderSize, height, color);
}

#undef drawByPixel