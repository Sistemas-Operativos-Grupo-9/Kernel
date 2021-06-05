#include <video.h>
#include <myUtils.h>
#include <lib.h>
// #include "Fonts/font.h"
#include "Fonts/RobotoMono-Regular.h"
#include <stdbool.h>

typedef struct {
    Color foreground;
    Color background;
} __attribute__((packed)) TextColor;

mode_info_block * const infoBlock = (mode_info_block *)0x0000000000005C00;
static int cursorX = 0;
static int cursorY = 0;
const uint32_t TEXT_WIDTH = 80;
const uint32_t TEXT_HEIGHT = 25;
Color * video;
uint32_t WIDTH;
uint32_t HEIGHT;
static TextColor currentColor = {.background = {0, 0, 0}, .foreground = {255, 255, 255}};

void initVideo() {
    video = (Color *) (uint64_t)infoBlock->physbase;
    WIDTH = infoBlock->x_res;
    HEIGHT = infoBlock->y_res;

    setCursorAt(0, 0);
}

void setForeground(Color color) {
    currentColor.foreground = color;
}
void setBackground(Color color) {
    currentColor.background = color;
}

void colorLerp(Color a, Color b, Color *out, uint8_t lerp) {
    *out = (Color) {
        .red = a.red + (b.red - a.red) * lerp / 255,
        .green = a.green + (b.green - a.green) * lerp / 255,
        .blue = a.blue + (b.blue - a.blue) * lerp / 255
    };
}

void setPixel(Color color, int x, int y) {
    video[x + y * WIDTH] = color;
    // unsigned char * pix = (unsigned char *) ((uint64_t) infoBlock->physbase + x*infoBlock->bpp / 8 + (int) y*infoBlock->pitch);
    // pix[0] = color.blue;
    // pix[1] = color.green;
    // pix[2] = color.red;
}
void setChar(char ch) {
    uint32_t *l = font[(uint8_t)ch];
    for (int y = 0; y < FONT_HEIGHT; y++) {
        for (int x = 0; x < FONT_WIDTH; x++) {
            Color color;
            int val = (l[y] >> ((FONT_WIDTH - x - 1) * FONT_BPP)) & 0b11;
            colorLerp(currentColor.background, currentColor.foreground, &color, val * (255 / 0b11));
            
            setPixel(color, cursorX * FONT_WIDTH + x, cursorY * FONT_HEIGHT + y);
        }
    }
}

void printChar(char ch) {
    if (ch == '\n') {
        setCursorAt(0, getCursorY() + 1);
    } else {
        setChar(ch);
        cursorX++;
        if (cursorX >= TEXT_WIDTH) {
            cursorX = 0;
            cursorY++;
        }
    }
}

void setCursorAt(uint32_t x, uint32_t y) {
    cursorX = x;
    cursorY = y;
}
uint32_t getCursorY() {
    return cursorY;
}
uint32_t getCursorX() {
    return cursorX;
}

void print(char *str) {
    while (*str) {
        printChar(*str++);
    }
}

void clear() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            setPixel(currentColor.background, x, y);
        }
    }

    // memset((char *) ((uint64_t) infoBlock->physbase), 0, HEIGHT * WIDTH * infoBlock->bpp/8);
}

void printIntN(int value, uint8_t digits, uint8_t base) {
    char str[digits + 1];
    numToString(value, digits, str, base);
    print(str);
}
void printInt(int value, uint8_t base) {
    printIntN(value, countDigits(value, base), base);
}

void printUnsignedN(unsigned value, uint8_t digits, uint8_t base) {
    char str[digits + 1];
    unsignedToString(value, digits, str, base);
    print(str);
}

void printUnsigned(unsigned value, uint8_t base) {
    printUnsignedN(value, countDigits(value, base), base);
}

void printHexPrefix() {
    printChar('0');
    printChar('x');
}

void printHexByte(uint8_t value) {
    printHexPrefix();
    printUnsignedN(value, 2, 16);
}

void printHexPointer(void *ptr) {
    printHexPrefix();
    printUnsignedN((uint64_t) ptr, 16, 16);
}

void printTestData() {
    print("Resolution: ");
    printUnsigned(WIDTH, 10);
    printChar('x');
    printUnsigned(HEIGHT, 10);
    printChar('\n');

    print("Video MEM: ");
    printHexPointer(video);
    printChar('\n');

    print("Video MEM: ");
    printHexPointer((void *)(uint64_t)infoBlock->physbase);
    printChar('\n');
    
}