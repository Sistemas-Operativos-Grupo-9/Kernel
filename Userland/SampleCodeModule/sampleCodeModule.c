/* sampleCodeModule.c */
#include "syscall.h"
#include <stdint.h>
#include <stdbool.h>
#include "keys.h"


char getch() {
	char ch;
	int r;
	do {
		r = read(0, &ch, 1);
	} while (r == 0);
	return ch;
}
void putchar(char ch) {
	write(0, &ch, 1);
}
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
void puts(char *str) {
	while (*str != '\0') {
		putchar(*str);
		str++;
	}
}

typedef struct {
	bool isPrintable;
	char data;
	enum Arrow arrow;
} keyStroke;

keyStroke readKeyStroke() {
	char first = getch();
	// If byte is scape
	if (first == ESC) {
		char second = getch();
		if (second == BRACKET) {
			char arrow = getch();
			return (keyStroke) {.isPrintable = false, .data = 0, .arrow = arrow};
		}
	}
	return (keyStroke) {.isPrintable = true, .data = first, .arrow = NO_ARROW};
}


int main() {

	uint32_t x = 0;
	puts("HOLA DESDE EL PROGRAMA!");
	while (true) {
		keyStroke c = readKeyStroke();
		if (c.isPrintable) {
			putchar(c.data);
		} else {
			if (c.arrow == ARROW_LEFT) {
				moveCursorLeft();
				x--;
			} else if (c.arrow == ARROW_RIGHT) {
				moveCursorRight();
				x++;
			}
		}
	}

	return 0xDEADBEEF;
}