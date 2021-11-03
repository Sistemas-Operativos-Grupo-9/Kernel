#pragma once

#include "Fonts/SourceCodePro-Regular.h"
#include "color.h"
#include <stdint.h>

typedef struct __attribute__((packed)) {
	uint16_t attributes;
	uint8_t win_A, win_B;
	uint16_t granularity;
	uint16_t winsize;
	uint16_t segment_A, segment_B;
	uint32_t real_fct_ptr;
	uint16_t pitch; // bytes per scanline

	uint16_t x_res, y_res;
	uint8_t w_char, y_char, planes, bpp, banks;
	uint8_t memory_model, bank_size, image_pages;
	uint8_t reserved0;

	uint8_t red_mask, red_position;
	uint8_t green_mask, green_position;
	uint8_t blue_mask, blue_position;
	uint8_t rsv_mask, rsv_position;
	uint8_t directcolor_attributes;

	uint32_t physbase; // your LFB (Linear Framebuffer) address ;)
	uint32_t reserved1;
	uint16_t reserved2;
} mode_info_block;

#define FONT_SCALE 1

#define FINAL_FONT_WIDTH (FONT_WIDTH * FONT_SCALE)
#define FINAL_FONT_HEIGHT (FONT_HEIGHT * FONT_SCALE)

uint32_t getWidth();
uint32_t getHeight();

uint16_t getOffsetX();
uint16_t getOffsetY();

Color colorLerp(Color a, Color b, uint8_t lerp);

void initVideo();

void drawCharAtFree(void(setPixel)(Color, uint16_t, uint16_t), char ch,
                    uint16_t x, uint16_t y, Color background, Color foreground);
void drawCharAt(char ch, uint8_t x, uint8_t y, Color background,
                Color foreground);
void setCharOffset(uint16_t widthCount, uint16_t heightCount);

void drawImage(Color (*getPixelColor)(uint16_t x, uint16_t y));
void drawCircleRaw(uint64_t centerX, uint64_t centerY, uint64_t radius,
                   Color color, uint64_t clipX, uint64_t clipY, uint64_t clipW,
                   uint64_t clipH);
void drawRectangleRaw(uint64_t xStart, uint64_t yStart, uint64_t width,
                      uint64_t height, Color color, uint64_t clipX,
                      uint64_t clipY, uint64_t clipW, uint64_t clipH);

void drawRectangleBorders(uint64_t xStart, uint64_t yStart, uint64_t width,
                          uint64_t height, uint16_t borderSize, Color color);

void drawBitmapRaw(uint64_t xStart, uint64_t yStart, uint64_t width,
                   uint64_t height, Color bitmap[][width], uint64_t clipX,
                   uint64_t clipY, uint64_t clipW, uint64_t clipH);