
#pragma once

#include "stdint.h"

void numToString(int num, uint8_t digits, char out[], uint8_t base);
uint64_t pow(unsigned base, unsigned exponent);
uint8_t countDigits(unsigned num, uint8_t base);
void unsignedToString(uint64_t num, uint8_t digits, char out[], uint8_t base);
