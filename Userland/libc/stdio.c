#include "stdio.h"
#include "myUtils.h"
#include "stdbool.h"
#include "syscall.h"
#include "syscalls.h"

void putCSI() {
	putchar(ESC);
	putchar(BRACKET);
}

void moveCursorRight() {
	putCSI();
	putchar(ARROW_RIGHT);
}

void moveCursorLeft() {
	putCSI();
	putchar(ARROW_LEFT);
}

void reset() {
	putchar(ESC);
	putchar('c');
}

void putchar(char ch) { write(1, &ch, 1); }

void puts(const char *str) {
	while (*str != '\0') {
		putchar(*str);
		str++;
	}
}

int getch(uint64_t timeout) {
	char ch = 0;
	if (read(0, &ch, 1, timeout) == 1)
		return ch;
	return TIMEOUT;
}

KeyStroke readKeyStroke(uint64_t timeout) {
#define getchReturnIfEOF(timeout)                                              \
	({                                                                         \
		int ch = getch(timeout);                                               \
		if (ch == EOF)                                                         \
			return (KeyStroke){.isEOF = true};                                 \
		if (ch == TIMEOUT)                                                     \
			return (KeyStroke){.isTimeout = true};                             \
		ch;                                                                    \
	})

	char first = getchReturnIfEOF(timeout);
	// If byte is scape
	if (first == ESC) {
		char second = getchReturnIfEOF(0);
		if (second == BRACKET) {
			char arrow = getchReturnIfEOF(0);
			return (KeyStroke){.isPrintable = false, .data = 0, .arrow = arrow};
		}
		// return (KeyStroke) {.isPrintable = false, .data = 0}
	}
	if (first == '\n' || first == '\b' || first == '\t') {
		return (KeyStroke){.isPrintable = false, .data = first};
	}
	return (KeyStroke){.isPrintable = true, .data = first};

#undef getchReturnIfEOF
}