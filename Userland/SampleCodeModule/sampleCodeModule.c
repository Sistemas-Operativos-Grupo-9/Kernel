/* sampleCodeModule.c */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>




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