#pragma once

#include "keys.h"
#include "stdbool.h"

typedef struct {
	bool isPrintable;
	char data;
	enum Arrow arrow;
	bool isEOF;
	bool isTimeout;
} KeyStroke;

void moveCursorRight();
void moveCursorLeft();
void reset();
void putchar(char ch);
void puts(const char *str);
int getch();
KeyStroke readKeyStroke(uint64_t timeout);