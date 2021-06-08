#pragma once

#include "keys.h"
#include "stdbool.h"

typedef struct {
	bool isPrintable;
	char data;
	enum Arrow arrow;
} keyStroke;

void putCSI();
void moveCursorRight();
void moveCursorLeft();
void putchar(char ch);
void puts(char *str);
char getch();
keyStroke readKeyStroke();