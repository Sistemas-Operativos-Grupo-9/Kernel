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
#define BLUE (Color) { 255, 0, 0 }
#define GREEN (Color) { 0, 255, 0 }
#define RED (Color) { 0, 0, 255 }

#define NORMAL_COLORS (struct TextColors) { .background = BLACK, .foreground = WHITE }

#define TEXT_WIDTH 83
#define TEXT_HEIGHT 29

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
} Views[] = {
    {.cursorX = 0, .cursorY = 0,
    .positionX = 0, .positionY = 0,
    .width = TEXT_WIDTH / 2, .height = TEXT_HEIGHT / 2,
    .scrollY = 0, .outputLength = 0},
    {.cursorX = 0, .cursorY = 0,
    .positionX = 0, .positionY = TEXT_HEIGHT / 2 + 1,
    .width = TEXT_WIDTH / 2, .height = TEXT_HEIGHT / 2,
    .scrollY = 0, .outputLength = 0},
    {.cursorX = 0, .cursorY = 0,
    .positionX = TEXT_WIDTH / 2 + 1, .positionY = 0,
    .width = TEXT_WIDTH / 2, .height = TEXT_HEIGHT / 2,
    .scrollY = 0, .outputLength = 0},
    {.cursorX = 0, .cursorY = 0,
    .positionX = TEXT_WIDTH / 2 + 1, .positionY = TEXT_HEIGHT / 2 + 1,
    .width = TEXT_WIDTH / 2, .height = TEXT_HEIGHT / 2,
    .scrollY = 0, .outputLength = 0}
};

uint8_t focusedView = 0;

void initColors() {
    for (int i = 0; i < sizeof(Views) / sizeof(*Views); i++)
        Views[i].colors = NORMAL_COLORS;
    for (int y = 0; y < TEXT_HEIGHT; y++) {
        for (int x = 0; x < TEXT_WIDTH; x++) {
            drawCharAt('*', x, y, BLACK, BLUE);
        }
    }
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
    Color foreground = view->colors.foreground;
    if (view - Views != focusedView)
        foreground = colorLerp(view->colors.background, view->colors.foreground, 64);
    drawCharAtView(view, view->textBuffer[y][x], x, y - view->scrollY, foreground, view->colors.background);
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

bool isPrintable(unsigned char ch) {
    return ((ch >= 0x20 && ch <= 0x7E) ||
        (ch >= 0x82 && ch <= 0x8C) ||
        (ch >= 0x91 && ch <= 0x9C) ||
        (ch == 0x9F) || (ch >= 0xA1));
}

void changeFocusView(uint8_t newFocusViewNumber) {
    struct View *view = &Views[newFocusViewNumber];
    focusedView = newFocusViewNumber;
    // Redraw cursors
    for (int i = 0; i < sizeof(Views) / sizeof(*Views); i++) {
        redrawCharInverted(&Views[i], Views[i].cursorX, Views[i].cursorY);
    }
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
            view->textBuffer[newCursorY][newCursorX] = ' ';
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
            redrawChar(view, x, y + view->scrollY);
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
    if (y < 0)
        y = 0;
    if (y >= view->scrollY + view->height) {
        if (y >= TEXT_BUFFER_HEIGHT) {
            int move = 10;
            int newY = y - move;
            int l = 0;
            for (; l < newY; l++) {
                int from = l + move, to = l;
                for (int i = 0; i < TEXT_BUFFER_WIDTH; i++) {
                    view->textBuffer[to][i] = view->textBuffer[from][i];
                }
            }
            for (; l < TEXT_BUFFER_HEIGHT; l++) {
                for (int i = 0; i < TEXT_BUFFER_WIDTH; i++) {
                    view->textBuffer[l][i] = ' ';
                }
            }
            y = newY;
        }
        view->scrollY = y - view->height + 1;
        reDraw(view);
    } else if (y < view->scrollY) {
        view->scrollY = y;
        reDraw(view);
    }
    if (view->cursorX != x || view->cursorY != y) {
        redrawChar(view, view->cursorX, view->cursorY);
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