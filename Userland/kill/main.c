
#include <shared-lib/print.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int main(int argc, char **argv) {
	if (argc != 1) {
		puts("Usage: kill <pid>\n");
		return -1;
	}

	uint64_t pid;
	if (!strtonum(argv[0], &pid, 10)) {
		return -1;
	}
	if (!kill(pid)) {
		puts("Process ");
		printUnsigned(pid, 10);
		puts(" does not exist.\n");
		return -2;
	}

	puts("Process ");
	printUnsigned(pid, 10);
	puts(" killed successfully.\n");
	return 0;
}
