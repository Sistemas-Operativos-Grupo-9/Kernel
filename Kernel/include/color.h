#pragma once
#include <stdint.h>

typedef struct {
    uint8_t red, green, blue;
} __attribute__((packed)) Color;