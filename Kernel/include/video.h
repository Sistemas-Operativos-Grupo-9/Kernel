#pragma once

#include <stdint.h>
#include "color.h"


void setForeground(Color color);
void setBackground(Color color);
uint32_t getCursorY();
uint32_t getCursorX();
void setCursorAt(int x, int y);
void print(char *str);
void printChar(char ch);
void printIntN(int value, uint8_t digits, uint8_t base);
void printInt(int value, uint8_t base);
void printUnsignedN(unsigned value, uint8_t digits, uint8_t base);
void printUnsigned(unsigned value, uint8_t base);
void printHexByte(uint8_t value);
void printHexPointer(void *ptr);
void clear();