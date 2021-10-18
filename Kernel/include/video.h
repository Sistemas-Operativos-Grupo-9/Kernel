#pragma once

#include "color.h"
#include <stdint.h>

extern uint8_t focusedView;

// void setForeground(Color color);
// void setBackground(Color color);
void initScreen();
uint32_t getCursorY();
uint32_t getCursorX();
void setCursorAt(uint8_t viewNumber, int x, int y);
void print(uint8_t viewNumber, char *str);
void printChar(uint8_t viewNumber, char ch);

void printIntN(uint8_t viewNumber, int value, uint8_t digits, uint8_t base);
void printInt(uint8_t viewNumber, int value, uint8_t base);
void printUnsignedN(uint8_t viewNumber, uint64_t value, uint8_t digits,
                    uint8_t base);
void printUnsigned(uint8_t viewNumber, uint64_t value, uint8_t base);
void printHexPrefix(uint8_t viewNumber);
void printHexByte(uint8_t viewNumber, uint8_t value);
void printHexPointer(uint8_t viewNumber, void *ptr);

void clear(uint8_t viewNumber);
void changeFocusView(uint8_t newFocusViewNumber);
void lookAround(uint8_t viewNumber, int deltaY);
int scrollTo(uint8_t viewNumber, int y);