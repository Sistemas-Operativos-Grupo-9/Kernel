#pragma once

#include "stdint.h"


void printIntN(int value, uint8_t digits, uint8_t base);
void printInt(int value, uint8_t base);
void printUnsignedN(unsigned value, uint8_t digits, uint8_t base);
void printUnsigned(unsigned value, uint8_t base);
void printHexPrefix();
void printHexByte(uint8_t value);
void printHexPointer(void *ptr);