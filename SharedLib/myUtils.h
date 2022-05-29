
#pragma once

#include "stdint.h"

void numToString(int num, uint8_t digits, char out[], uint8_t base);
uint64_t pow(unsigned base, unsigned exponent);
unsigned countDigits(int num, unsigned base);
void unsignedToString(uint64_t num, uint8_t digits, char out[], uint8_t base);
