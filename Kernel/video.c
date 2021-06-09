#include <video.h>
#include <myUtils.h>
#include <lib.h>
#include <stdbool.h>
#include "keys.h"
#include "basicVideo.h"
#include "process.h"


struct TextColors {
    Color foreground;
    Color background;
} __attribute__((packed));// currentColor = {.background = {0, 0, 0}, .foreground = {255, 255, 255}};

#define BLACK (Color) { 0, 0, 0 }
#define WHITE (Color) { 255, 255, 255 }

#define NORMAL_COLORS (struct TextColors) { .background = BLACK, .foreground = WHITE }

#define TEXT_WIDTH 80
#define TEXT_HEIGHT 25

#define TEXT_BUFFER_WIDTH TEXT_WIDTH
#define TEXT_BUFFER_HEIGHT TEXT_HEIGHT * 5

static struct View {
    int cursorX;
    int cursorY;
    int scrollY; // First line being shown from textBuffer
    struct TextColors colors;
    char textBuffer[TEXT_BUFFER_HEIGHT][TEXT_BUFFER_WIDTH];
    char outputBuffer[256];
    uint32_t outputLength;
    uint32_t positionX, positionY;
    uint32_t width, height;
} Views[2] = {
    {.cursorX = 0, .cursorY = 0,
    .positionX = 0, .positionY = 0,
    .width = TEXT_WIDTH, .height = TEXT_HEIGHT / 2,
    .scrollY = 0, .outputLength = 0},
    {.cursorX = 0, .cursorY = 0,
    .positionX = 0, .positionY = TEXT_HEIGHT / 2,
    .width = TEXT_WIDTH, .height = TEXT_HEIGHT / 2,
    .scrollY = 0, .outputLength = 0}
};

void initColors() {
    Views[0].colors = NORMAL_COLORS;
    Views[1].colors = NORMAL_COLORS;
}

void setForeground(struct View *view, Color color) {
    view->colors.foreground = color;
}
void setBackground(struct View *view, Color color) {
    view->colors.background = color;
}

void drawCharAtView(struct View *view, char ch, uint8_t x, uint8_t y, Color background, Color foreground) {
    // if (x >= view->width)
    drawCharAt(ch, x + view->positionX, y + view->positionY, background, foreground);
}
Color invertColor(Color color) {
    return (Color) {.red = 255 - color.red, .green = 255 - color.green, .blue = 255 - color.blue};
}
void redrawCharInverted(struct View *view, uint32_t x, uint32_t y) {
    drawCharAtView(view, view->textBuffer[y][x], x, y - view->scrollY, view->colors.foreground, view->colors.background);
}
void redrawCharUnfocused(struct View *view, uint32_t x, uint32_t y) {
    drawCharAtView(view, view->textBuffer[y][x], x, y - view->scrollY, view->colors.foreground, colorLerp(view->colors.foreground, view->colors.background, 128));
}
void redrawChar(struct View *view, uint32_t x, uint32_t y) {
    drawCharAtView(view, view->textBuffer[y][x], x, y - view->scrollY, view->colors.background, view->colors.foreground);
}
void setChar(struct View *view, char ch) {
    view->textBuffer[view->cursorY][view->cursorX] = ch;
    redrawChar(view, view->cursorX, view->cursorY);
}


void consume(struct View *view, int count) {
    view->outputLength -= count;
    for (int i = 0; i < view->outputLength; i++) {
        view->outputBuffer[i] = view->outputBuffer[i + count];
    }
}

bool isPrintable(char ch) {
    return ((ch >= 0x20 && ch <= 0x7E) ||
        (ch >= 0x82 && ch <= 0x8C) ||
        (ch >= 0x91 && ch <= 0x9C) ||
        (ch == 0x9F) || (ch >= 0xA1));
}

void changeFocusView(uint8_t newFocusViewNumber) {
    struct View *view = &Views[newFocusViewNumber];
    for (int i = 0; i < sizeof(Views) / sizeof(*Views); i++) {
        redrawChar(&Views[i], Views[i].cursorX, Views[i].cursorY);
    }
    redrawCharInverted(view, view->cursorX, view->cursorY);
}

void printChar(uint8_t viewNumber, char ch) {
    struct View *view = &Views[viewNumber];
    view->outputBuffer[view->outputLength++] = ch;

    int newCursorX = view->cursorX, newCursorY = view->cursorY;

    bool finish = false;
    while (!finish && view->outputLength > 0) {
        finish = true;
        if (view->outputBuffer[0] == ESC) {
            if (view->outputLength > 1 && view->outputBuffer[1] == BRACKET) {
                if (view->outputLength > 2) {
                    enum Arrow arrow = view->outputBuffer[2];
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
                    consume(view, 3);
                    finish = false;
                }
            }
        } else if (view->outputBuffer[0] == '\n') {
            newCursorX = 0;
            newCursorY++;
            consume(view, 1);
            finish = false;
        } else if (view->outputBuffer[0] == '\b') {
            newCursorX--;
            setCursorAt(viewNumber, newCursorX, newCursorY);
            setChar(view, ' ');
            redrawCharInverted(view, view->cursorX, view->cursorY);
            consume(view, 1);
            finish = false;
        } else if (isPrintable(view->outputBuffer[0])) {
            setChar(view, view->outputBuffer[0]);
            consume(view, 1);
            newCursorX++;
            finish = false;
        } else {
            consume(view, 1);
            finish = false;
        }
    }
    setCursorAt(viewNumber, newCursorX, newCursorY);
}

void reDraw(struct View *view) {

    for (int y = 0; y < view->height; y++) {
        for (int x = 0; x < view->width; x++) {
            drawCharAtView(view, view->textBuffer[y + view->scrollY][x], x, y, view->colors.background, view->colors.foreground);
        }
    }
    redrawCharInverted(view, view->cursorX, view->cursorY);
}

void setCursorAt(uint8_t viewNumber, int x, int y) {
    struct View *view = &Views[viewNumber];
    
    while (x >= (int)view->width) {
        x = 0;
        y++;
    }
    while (x < 0) {
        x = view->width - 1;
        y--;
    }
    if (y >= view->scrollY + view->height) {
        view->scrollY = y - view->height + 1;
        reDraw(view);
    } else if (y < view->scrollY) {
        view->scrollY = y;
        reDraw(view);
    }
    if (view->cursorX != x || view->cursorY != y) {
        redrawChar(view, view->cursorX, view->cursorY);
        if (getFocusedProcess().tty == viewNumber)
            redrawCharInverted(view, x, y);
    }
    view->cursorX = x;
    view->cursorY = y;
}
// uint32_t getCursorY() {
//     return cursorY;
// }
// uint32_t getCursorX() {
//     return cursorX;
// }

void print(uint8_t viewNumber, char *str) {
    while (*str) {
        printChar(viewNumber, *str++);
    }
}

void clear(uint8_t viewNumber) {
    struct View *view = &Views[viewNumber];

    view->scrollY = 0;
    for (int y = 0; y < TEXT_BUFFER_HEIGHT; y++) {
        for (int x = 0; x < TEXT_BUFFER_WIDTH; x++) {
            view->textBuffer[y][x] = ' ';
        }
    }
    view->cursorX = 0;
    view->cursorY = 0;
    reDraw(view);
}

// Helper print functions for Kernel space
void printIntN(int value, uint8_t digits, uint8_t base) {
    char str[digits + 1];
    numToString(value, digits, str, base);
    print(0, str);
}
void printInt(int value, uint8_t base) {
    printIntN(value, countDigits(value, base), base);
}

void printUnsignedN(unsigned value, uint8_t digits, uint8_t base) {
    char str[digits + 1];
    unsignedToString(value, digits, str, base);
    print(0, str);
}

void printUnsigned(unsigned value, uint8_t base) {
    printUnsignedN(value, countDigits(value, base), base);
}

void printHexPrefix() {
    printChar(0, '0');
    printChar(0, 'x');
}

void printHexByte(uint8_t value) {
    printHexPrefix();
    printUnsignedN(value, 2, 16);
}

void printHexPointer(void *ptr) {
    printHexPrefix();
    printUnsignedN((uint64_t) ptr, 16, 16);
}