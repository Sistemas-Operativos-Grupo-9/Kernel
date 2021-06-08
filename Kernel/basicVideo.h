#pragma once


#include <stdint.h>
#include "color.h"

typedef struct __attribute__((packed)) {
  uint16_t attributes;
  uint8_t win_A,win_B;
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

  uint32_t physbase;  // your LFB (Linear Framebuffer) address ;)
  uint32_t reserved1;
  uint16_t reserved2;
} mode_info_block;

uint32_t getWidth();
uint32_t getHeight();

void initVideo();
void drawCharAt(char ch, uint8_t x, uint8_t y, Color background, Color foreground);