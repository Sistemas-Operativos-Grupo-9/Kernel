#include "monogram.h"
uint8_t monogram_letters[188][12] = {
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xa, 0xa, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0xa, 0x1f, 0xa, 0xa, 0x1f, 0xa, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0xf, 0x14, 0xe, 0x5, 0x1e, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x11, 0x2, 0x4, 0x8, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xf, 0x12, 0x12, 0xd, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x4, 0x4, 0x4, 0x2, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x8, 0x4, 0x4, 0x4, 0x4, 0x4, 0x8, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x4, 0x15, 0xe, 0x15, 0x4, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x1f, 0x4, 0x4, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x8, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1, 0x1, 0x2, 0x4, 0x8, 0x10, 0x10, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x13, 0x15, 0x19, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0xc, 0x4, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x1, 0x2, 0x4, 0x8, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x1, 0x6, 0x1, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x9, 0x9, 0x11, 0x1f, 0x1, 0x1, 0x1, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1f, 0x10, 0x1e, 0x1, 0x1, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x10, 0x10, 0x1e, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1f, 0x1, 0x1, 0x2, 0x4, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x11, 0xe, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x11, 0xf, 0x1, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0, 0x4, 0x4, 0x8, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x3, 0xc, 0x10, 0xc, 0x3, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x0, 0x1f, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x18, 0x6, 0x1, 0x6, 0x18, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x1, 0x2, 0x4, 0x0, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x13, 0x15, 0x15, 0x13, 0x10, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x10, 0x10, 0x10, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1e, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x10, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x10, 0x17, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1f, 0x4, 0x4, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x1b, 0x15, 0x11, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x3, 0x0},
    {0x0, 0x0, 0x0, 0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x10, 0xe, 0x1, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1f, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x11, 0xa, 0xa, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x11, 0x15, 0x1b, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x11, 0xa, 0x4, 0xa, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0x11, 0xa, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1f, 0x1, 0x2, 0x4, 0x8, 0x10, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x6, 0x4, 0x4, 0x4, 0x4, 0x4, 0x6, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x10, 0x10, 0x8, 0x4, 0x2, 0x1, 0x1, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xc, 0x4, 0x4, 0x4, 0x4, 0x4, 0xc, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0xa, 0x11, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x8, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x10, 0x10, 0x1e, 0x11, 0x11, 0x11, 0x1e, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x11, 0x10, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1, 0x1, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x11, 0x1f, 0x10, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x6, 0x9, 0x8, 0x1e, 0x8, 0x8, 0x8, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x1, 0xe},
    {0x0, 0x0, 0x0, 0x10, 0x10, 0x1e, 0x11, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x0, 0xc, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1, 0x0, 0x3, 0x1, 0x1, 0x1, 0x1, 0x11, 0xe},
    {0x0, 0x0, 0x0, 0x10, 0x10, 0x11, 0x12, 0x1c, 0x12, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x18, 0x8, 0x8, 0x8, 0x8, 0x8, 0x7, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x1e, 0x15, 0x15, 0x15, 0x15, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x1e, 0x11, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x1e, 0x11, 0x11, 0x11, 0x1e, 0x10, 0x10},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x1, 0x1},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x16, 0x19, 0x10, 0x10, 0x10, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x10, 0xe, 0x1, 0x1e, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x8, 0x8, 0x1e, 0x8, 0x8, 0x8, 0x7, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0xa, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x11, 0x11, 0x15, 0x15, 0xa, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x11, 0xa, 0x4, 0xa, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x11, 0xf, 0x1, 0xe},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x2, 0x4, 0x8, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x8, 0x4, 0x4, 0x2, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x8, 0x4, 0x4, 0x2, 0x4, 0x4, 0x8, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x9, 0x16, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0xe, 0x15, 0x14, 0x15, 0xe, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x6, 0x9, 0x8, 0x1e, 0x8, 0x8, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x11, 0xe, 0xa, 0xe, 0x11, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x11, 0xa, 0x4, 0x1f, 0x4, 0x1f, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x4, 0x4, 0x0, 0x4, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xf, 0x10, 0xe, 0x11, 0xe, 0x1, 0x1e, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x1b, 0x15, 0x17, 0x15, 0x1b, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x12, 0x12, 0x12, 0xe, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x9, 0x12, 0x9, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x1, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x13, 0x15, 0x15, 0x13, 0x15, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x4, 0x1f, 0x4, 0x4, 0x0, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x18, 0x4, 0x8, 0x10, 0x1c, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x18, 0x4, 0x8, 0x4, 0x18, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x2, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x11, 0x1e, 0x10, 0x10},
    {0x0, 0x0, 0x0, 0xf, 0x1d, 0x1d, 0x1d, 0xd, 0x5, 0x5, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x2, 0xc},
    {0x0, 0x0, 0x0, 0x8, 0x18, 0x8, 0x8, 0x1c, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0x12, 0xc, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x12, 0x9, 0x12, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x10, 0x12, 0x14, 0x8, 0x15, 0x7, 0x1, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x10, 0x12, 0x14, 0xe, 0x11, 0x2, 0x7, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1c, 0xd, 0x1e, 0x4, 0xd, 0x17, 0x1, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x4, 0x0, 0x4, 0x8, 0x10, 0x11, 0xe, 0x0, 0x0},
    {0x8, 0x4, 0x0, 0xe, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x2, 0x4, 0x0, 0xe, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x4, 0xa, 0x0, 0xe, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0xd, 0x12, 0x0, 0xe, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0xa, 0x0, 0xe, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x4, 0xa, 0x4, 0xe, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xf, 0x14, 0x14, 0x1f, 0x14, 0x14, 0x17, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xe, 0x11, 0x10, 0x10, 0x10, 0x11, 0xe, 0x2, 0xc},
    {0x8, 0x4, 0x0, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f, 0x0, 0x0},
    {0x2, 0x4, 0x0, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f, 0x0, 0x0},
    {0x4, 0xa, 0x0, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f, 0x0, 0x0},
    {0x0, 0xa, 0x0, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f, 0x0, 0x0},
    {0x8, 0x4, 0x0, 0x1f, 0x4, 0x4, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x2, 0x4, 0x0, 0x1f, 0x4, 0x4, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x4, 0xa, 0x0, 0x1f, 0x4, 0x4, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0xa, 0x0, 0x1f, 0x4, 0x4, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x1e, 0x11, 0x11, 0x19, 0x11, 0x11, 0x1e, 0x0, 0x0},
    {0xd, 0x12, 0x0, 0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x0, 0x0},
    {0x8, 0x4, 0x0, 0xe, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x2, 0x4, 0x0, 0xe, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x4, 0xa, 0x0, 0xe, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0xd, 0x12, 0x0, 0xe, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0xa, 0x0, 0xe, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa, 0x4, 0xa, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xd, 0x12, 0x13, 0x15, 0x19, 0x9, 0x16, 0x0, 0x0},
    {0x8, 0x4, 0x0, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x2, 0x4, 0x0, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x4, 0xa, 0x0, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0xa, 0x0, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x2, 0x4, 0x0, 0x11, 0x11, 0xa, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x10, 0x1e, 0x11, 0x11, 0x11, 0x1e, 0x10, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0x16, 0x11, 0x11, 0x16, 0x0, 0x0},
    {0x0, 0x0, 0x8, 0x4, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x2, 0x4, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x4, 0xa, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0xd, 0x12, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xa, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x4, 0xa, 0x4, 0x0, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x15, 0x17, 0x14, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x11, 0x10, 0x11, 0xe, 0x2, 0xc},
    {0x0, 0x0, 0x8, 0x4, 0x0, 0xe, 0x11, 0x1f, 0x10, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x2, 0x4, 0x0, 0xe, 0x11, 0x1f, 0x10, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x4, 0xa, 0x0, 0xe, 0x11, 0x1f, 0x10, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xa, 0x0, 0xe, 0x11, 0x1f, 0x10, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x8, 0x4, 0x0, 0xc, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x2, 0x4, 0x0, 0xc, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x4, 0xa, 0x0, 0xc, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xa, 0x0, 0xc, 0x4, 0x4, 0x4, 0x1f, 0x0, 0x0},
    {0x0, 0x0, 0xe, 0x3, 0x3, 0xf, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0xd, 0x12, 0x0, 0x1e, 0x11, 0x11, 0x11, 0x11, 0x0, 0x0},
    {0x0, 0x0, 0x8, 0x4, 0x0, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x2, 0x4, 0x0, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x4, 0xa, 0x0, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0xd, 0x12, 0x0, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xa, 0x0, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x1f, 0x0, 0x4, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0xd, 0x12, 0x15, 0x9, 0x16, 0x0, 0x0},
    {0x0, 0x0, 0x8, 0x4, 0x0, 0x11, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x2, 0x4, 0x0, 0x11, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x4, 0xa, 0x0, 0x11, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0xa, 0x0, 0x11, 0x11, 0x11, 0x11, 0xf, 0x0, 0x0},
    {0x0, 0x0, 0x2, 0x4, 0x0, 0x11, 0x11, 0x11, 0x11, 0xf, 0x1, 0xe},
    {0x0, 0x0, 0x0, 0x10, 0x10, 0x1e, 0x11, 0x11, 0x11, 0x1e, 0x10, 0x10},
    {0x0, 0x0, 0x0, 0xa, 0x0, 0x11, 0x11, 0x11, 0x11, 0xf, 0x1, 0xe}
};
uint8_t monogram_mapping[256] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x0, 0x6b, 0xd, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb};