
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
                    setPixel(color, x * (FONT_WIDTH * FONT_SCALE) + (fontX * FONT_SCALE + sx), y * (FONT_HEIGHT * FONT_SCALE) + (fontY * FONT_SCALE + sy));
                }
            }
        }
    }
}