#pragma once

#include "stdint.h"

void printIntN(int value, uint8_t digits, uint8_t base);
void printInt(int value, uint8_t base);
void printUnsignedN(uint64_t value, uint8_t digits, uint8_t base);
void printUnsigned(uint64_t value, uint8_t base);
void printHexPrefix();
void printHexByte(uint8_t value);
void printHexPointer(void *ptr);
void printPercentage(float value);