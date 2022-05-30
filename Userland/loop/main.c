#include <shared-lib/print.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int main(int argc, char **argv) {
	while (true) {
		puts("Soy pid=");
		printInt(getpid(), 10);
		putchar('\n');
		for (int i = 0; i < 100; i++);
	}

	return 0;
}
