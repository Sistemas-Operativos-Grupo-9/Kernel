#pragma once
#include "stdbool.h"
#include "stdint.h"

bool strtonum(char *num, uint64_t *out, uint8_t base);