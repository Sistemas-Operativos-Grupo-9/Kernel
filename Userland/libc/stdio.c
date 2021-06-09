#include "syscall.h"
#include "stdio.h"
#include "stdbool.h"

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
	write(0, &ch, 1);
}

void puts(char *str) {
	while (*str != '\0') {
		putchar(*str);
		str++;
	}
}

char getch() {
	char ch;
	int r;
	do {
		r = read(0, &ch, 1);
	} while (r == 0);
	return ch;
}

KeyStroke readKeyStroke() {
	char first = getch();
	// If byte is scape
	if (first == ESC) {
		char second = getch();
		if (second == BRACKET) {
			char arrow = getch();
			return (KeyStroke) {.isPrintable = false, .data = 0, .arrow = arrow};
		}
	}
	return (KeyStroke) {.isPrintable = true, .data = first, .arrow = NO_ARROW};
}