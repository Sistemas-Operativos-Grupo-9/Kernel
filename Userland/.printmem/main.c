#include <stdio.h>
#include <stdlib.h>
#include <shared-lib/print.h>

int main(int argc, char **argv) {
	if (argc != 1) {
		puts("Usage: printmem <pointer (without 0x)>\n");
		return -1;
	}

	uint64_t *pointer;
	if (!strtonum(argv[0], (uint64_t *)&pointer, 16)) {
		return -2;
	}
	printHexPointer(pointer);
	puts(":\n");

	for (int i = 0; i < 4; i++) {
		puts("    ");
		printHexPointer((void *)(*pointer++));
		putchar('\n');
	}

	return 0;
}
