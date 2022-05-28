#pragma once

#include "keys.h"
#include <stdbool.h>
#include <stdint.h>

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
int getch();
KeyStroke readKeyStroke(uint64_t timeout);
