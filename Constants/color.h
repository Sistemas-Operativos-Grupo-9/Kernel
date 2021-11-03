#pragma once
#include <stdint.h>

typedef struct {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
} __attribute__((packed)) Color;

#define BLACK                                                                  \
	(Color) { 0, 0, 0 }
#define WHITE                                                                  \
	(Color) { 255, 255, 255 }
#define BLUE                                                                   \
	(Color) { 255, 0, 0 }
#define GREEN                                                                  \
	(Color) { 0, 255, 0 }
#define RED                                                                    \
	(Color) { 0, 0, 255 }
#define ORANGE                                                                 \
	(Color) { 0, 150, 255 }
#define AQUA                                                                   \
	(Color) { 255, 255, 0 }
#define LIGHT_SALMON                                                           \
	(Color) { 122, 160, 255 }
#define GREEN_YELLOW                                                           \
	(Color) { 47, 255, 173 }
#define GREY_N3                                                                \
	(Color) { 50, 50, 50 }
#define GREY_N2                                                                \
	(Color) { 100, 100, 100 }
#define GREY_N1                                                                \
	(Color) { 150, 150, 150 }
#define GREY                                                                   \
	(Color) { 200, 200, 200 }
#define GREY_1                                                                 \
	(Color) { 215, 215, 215 }
#define GREY_2                                                                 \
	(Color) { 230, 230, 230 }
#define GREY_3                                                                 \
	(Color) { 240, 240, 240 }
