#include <video.h>
#include <myUtils.h>
#include <lib.h>
#include <stdbool.h>
#include "keys.h"
#include "basicVideo.h"


struct {
    Color foreground;
    Color background;
} __attribute__((packed)) currentColor = {.background = {0, 0, 0}, .foreground = {255, 255, 255}};

static int cursorX = 0;
static int cursorY = 0;

#define TEXT_WIDTH 80
#define TEXT_HEIGHT 25

#define TEXT_BUFFER_WIDTH TEXT_WIDTH
#define TEXT_BUFFER_HEIGHT TEXT_HEIGHT * 10

char TEXT_BUFFER[TEXT_BUFFER_HEIGHT][TEXT_BUFFER_WIDTH];
int scrollY = 0; // First line being shown from TEXT_BUFFER


void setForeground(Color color) {
    currentColor.foreground = color;
}
void setBackground(Color color) {
    currentColor.background = color;
}

Color invertColor(Color color) {
    return (Color) {.red = 255 - color.red, .green = 255 - color.green, .blue = 255 - color.blue};
}
void redrawCharInverted(uint32_t x, uint32_t y) {
    drawCharAt(TEXT_BUFFER[y][x], x, y - scrollY, currentColor.foreground, currentColor.background);
}
void redrawChar(uint32_t x, uint32_t y) {
    drawCharAt(TEXT_BUFFER[y][x], x, y - scrollY, currentColor.background, currentColor.foreground);
}
void setChar(char ch) {
    TEXT_BUFFER[cursorY][cursorX] = ch;
    redrawChar(cursorX, cursorY);
}


static char output_buffer[256];
static int output_length = 0;
void consume(int count) {
    output_length -= count;
    for (int i = 0; i < output_length; i++) {
        output_buffer[i] = output_buffer[i + count];
    }
}

bool isPrintable(char ch) {
    return ((ch >= 0x20 && ch <= 0x7E) ||
        (ch >= 0x82 && ch <= 0x8C) ||
        (ch >= 0x91 && ch <= 0x9C) ||
        (ch == 0x9F) || (ch >= 0xA1));
}

void printChar(char ch) {
    output_buffer[output_length++] = ch;

    int newCursorX = cursorX, newCursorY = cursorY;

    bool finish = false;
    while (!finish && output_length > 0) {
        finish = true;
        if (output_buffer[0] == ESC) {
            if (output_length > 1 && output_buffer[1] == BRACKET) {
                if (output_length > 2) {
                    enum Arrow arrow = output_buffer[2];
                    switch (arrow)
                    {
                    case ARROW_LEFT:
                        newCursorX--;
                        break;
                    case ARROW_RIGHT:
                        newCursorX++;
                        break;
                    default:
                        break;
                    }
                    consume(3);
                    finish = false;
                }
            }
        } else if (output_buffer[0] == '\n') {
            newCursorX = 0;
            newCursorY++;
            consume(1);
            finish = false;
        } else if (output_buffer[0] == '\b') {
            newCursorX--;
            setCursorAt(newCursorX, newCursorY);
            setChar(' ');
            redrawCharInverted(cursorX, cursorY);
            consume(1);
            finish = false;
        } else if (isPrintable(output_buffer[0])) {
            setChar(output_buffer[0]);
            consume(1);
            newCursorX++;
            finish = false;
        } else {
            consume(1);
            finish = false;
        }
    }
    setCursorAt(newCursorX, newCursorY);
}

void reDraw() {

    for (int y = 0; y < TEXT_HEIGHT; y++) {
        for (int x = 0; x < TEXT_WIDTH; x++) {
            drawCharAt(TEXT_BUFFER[y + scrollY][x], x, y, currentColor.background, currentColor.foreground);
        }
    }
    redrawCharInverted(cursorX, cursorY);
}

void setCursorAt(int x, int y) {
    while (x >= (int)TEXT_WIDTH) {
        x = 0;
        y++;
    }
    while (x < 0) {
        x = TEXT_WIDTH - 1;
        y--;
    }
    if (y >= scrollY + TEXT_HEIGHT) {
        scrollY = y - TEXT_HEIGHT + 1;
        reDraw();
    } else if (y < scrollY) {
        scrollY = y;
        reDraw();
    }
    if (cursorX != x || cursorY != y) {
        redrawChar(cursorX, cursorY);
        redrawCharInverted(x, y);
    }
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
    scrollY = 0;
    for (int y = 0; y < TEXT_BUFFER_HEIGHT; y++) {
        for (int x = 0; x < TEXT_BUFFER_WIDTH; x++) {
            TEXT_BUFFER[y][x] = ' ';
        }
    }
    cursorX = 0;
    cursorY = 0;
    reDraw();
}

// Helper print functions for Kernel space
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