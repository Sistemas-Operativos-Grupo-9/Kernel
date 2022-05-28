#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <shared-lib/print.h>

int main(int argc, char **argv) {
	if (argc == 0) {
		return -1;
	}
	char *program = argv[0];
	uint64_t interval = 1000;
	if (argc >= 2) {
		if (!strtonum(argv[1], &interval, 10))
			return -1;
	}

	KeyStroke key;
	do {
		reset();
		char **args = NULL;
		if (execve(program, args)) {
			puts("Program ");
			puts(program);
			puts(" does not exist.");
		}
		key = readKeyStroke(interval);
	} while (key.data != 'q');

	return 0;
}
