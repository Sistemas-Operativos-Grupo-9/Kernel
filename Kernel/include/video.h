#pragma once

#include <stdint.h>
#include "color.h"

extern uint8_t focusedView;

// void setForeground(Color color);
// void setBackground(Color color);
void initColors();
uint32_t getCursorY();
uint32_t getCursorX();
void setCursorAt(uint8_t viewNumber, int x, int y);
void print(uint8_t viewNumber, char *str);
void printChar(uint8_t viewNumber, char ch);
void printIntN(int value, uint8_t digits, uint8_t base);
void printInt(int value, uint8_t base);
void printUnsignedN(unsigned value, uint8_t digits, uint8_t base);
void printUnsigned(unsigned value, uint8_t base);
void printHexByte(uint8_t value);
void printHexPointer(void *ptr);
void clear(uint8_t viewNumber);
void changeFocusView(uint8_t newFocusViewNumber);