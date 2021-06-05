#include <video.h>
#include <myUtils.h>


typedef struct {
    enum Color foreground : 4;
    enum Color background : 4;
} __attribute__((packed)) Color;

typedef struct {
    uint8_t ch;
    Color color;
} Cell;

Cell * const video = (Cell *)0xB8000;
static Cell *currentChar = (Cell *)0xB8000;
const uint32_t WIDTH = 80;
const uint32_t HEIGHT = 25;
static Color currentColor = {.background = BLACK, .foreground = WHITE};

void setForeground(enum Color color) {
    currentColor.foreground = color;
}
void setBackground(enum Color color) {
    currentColor.background = color;
}

void setChar(char ch) {
    currentChar->ch = ch;
    currentChar->color = currentColor;
}

void printChar(char ch) {
    if (ch == '\n') {
        setCursorAt(0, getCursorY() + 1);
    } else {
        setChar(ch);
        currentChar++;
    }
}

void setCursorAt(uint32_t x, uint32_t y) {
    currentChar = video + y * WIDTH + x;
}
uint32_t getCursorY() {
    return (currentChar - video) / WIDTH;
}
uint32_t getCursorX() {
    return (currentChar - video) % WIDTH;
}

void print(char *str) {
    while (*str) {
        printChar(*str++);
    }
}

void clear() {
    setBackground(BLACK);
    setForeground(WHITE);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            setCursorAt(x, y);
            printChar(' ');
        }
    }
}

void printInt(int value, uint8_t digits, uint8_t base) {
    char str[digits + 1];
    numToString(value, digits, str, base);
    print(str);
}

void printUnsigned(unsigned value, uint8_t digits, uint8_t base) {
    char str[digits + 1];
    unsignedToString(value, digits, str, base);
    print(str);
}

void printHexByte(uint8_t value) {
    printChar('0');
    printChar('x');
    printUnsigned(value, 2, 16);
}