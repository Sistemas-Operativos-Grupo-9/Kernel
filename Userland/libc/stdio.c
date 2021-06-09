#include "syscall.h"
#include "stdio.h"
#include "stdbool.h"
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

void putchar(char ch) {
	write(1, &ch, 1);
}

void puts(char *str) {
	while (*str != '\0') {
		putchar(*str);
		str++;
	}
}

int getch() {
	char ch;
	int r;
	do {
		r = read(0, &ch, 1);
	} while (r == 0);
	return ch;
}

KeyStroke readKeyStroke() {
	#define getchReturnIfEOF()  ({																			\
		int ch = getch(); 																					\
		if (ch == EOF) return (KeyStroke) {.isEOF = true, .isPrintable = false, .arrow = NO_ARROW}; 		\
		ch;})

	char first = getchReturnIfEOF();
	// If byte is scape
	if (first == ESC) {
		char second = getchReturnIfEOF();
		if (second == BRACKET) {
			char arrow = getchReturnIfEOF();
			return (KeyStroke) {.isPrintable = false, .data = 0, .arrow = arrow};
		}
		// return (KeyStroke) {.isPrintable = false, .data = 0}
	}
	return (KeyStroke) {.isPrintable = true, .data = first, .arrow = NO_ARROW};

	#undef getchReturnIfEOF
}