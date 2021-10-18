
#include <print.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int main(char **argv, int argc) {
	if (argc != 1) {
		puts("Usage: kill <pid>\n");
		return -1;
	}

	uint64_t pid = strtonum(argv[0], 10);
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