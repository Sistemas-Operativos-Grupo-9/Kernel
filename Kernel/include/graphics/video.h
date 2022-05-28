#pragma once

#include "color.h"
#include "window.h"
#include <stdbool.h>
#include <stdint.h>

extern uint8_t focusedView;

// void setForeground(Color color);
// void setBackground(Color color);
void initScreen();
uint32_t getCursorY();
uint32_t getCursorX();
void setCursorAt(uint8_t viewNumber, int x, int y);
void ttyPuts(uint8_t viewNumber, const char *str);
void ttyPutchar(uint8_t viewNumber, char ch);

void setForeground(uint8_t viewNumber, Color color);
void setBackground(uint8_t viewNumber, Color color);

//void printIntN(uint8_t viewNumber, int value, uint8_t digits, uint8_t base);
//void printInt(uint8_t viewNumber, int value, uint8_t base);
//void printUnsignedN(uint8_t viewNumber, uint64_t value, uint8_t digits,
                    //uint8_t base);
//void printUnsigned(uint8_t viewNumber, uint64_t value, uint8_t base);
//void printHexPrefix(uint8_t viewNumber);
//void printHexByte(uint8_t viewNumber, uint8_t value);
//void printHexPointer(uint8_t viewNumber, void *ptr);

void setViewGraphic(uint8_t viewNumber, bool value);
void clear(uint8_t viewNumber);
void changeFocusView(uint8_t newFocusViewNumber);
void lookAround(uint8_t viewNumber, int deltaY);
int scrollTo(uint8_t viewNumber, int y);

void drawCircle(uint8_t viewNumber, uint16_t x, uint16_t y, uint16_t radius);
void drawRectangle(uint8_t viewNumber, uint16_t x, uint16_t y, uint16_t width,
                   uint16_t height);
void getViewInfo(uint8_t viewNumber, WindowInfo *windowInfo);

void drawBitmap(uint8_t viewNumber, uint16_t x, uint16_t y, uint16_t width,
                uint16_t height, Color bitmap[][width]);
void drawText(uint8_t viewNumber, char *text, uint16_t x, uint16_t y,
              bool center);
void drawLine(uint8_t viewNumber, uint16_t x1, uint16_t y1, uint16_t x2,
              uint16_t y2);
void flip(uint8_t viewNumber);

void focusNextView();
void focusDesktop(int desktopNumber);
