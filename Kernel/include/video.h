#pragma once

#include <stdint.h>

enum Color {
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PURPLE,
    BROWN,
    BRIGHT_GREY,
    GREY,
    NEON_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    PINK,
    YELLOW,
    WHITE
};

void setForeground(enum Color color);
void setBackground(enum Color color);
uint32_t getCursorY();
uint32_t getCursorX();
void setCursorAt(uint32_t x, uint32_t y);
void print(char *str);
void printChar(char ch);
void printInt(int value, uint8_t digits, uint8_t base);
void printUnsigned(unsigned value, uint8_t digits, uint8_t base);
void clear();