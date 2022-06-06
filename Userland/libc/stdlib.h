#pragma once
#include "stdbool.h"
#include "stdint.h"

bool strtonum(const char *num, uint64_t *out, uint8_t base);
bool strtoint(const char *num, int64_t *out, uint8_t base);
