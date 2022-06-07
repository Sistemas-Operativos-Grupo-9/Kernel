
#include <shared-lib/print.h>
#include <string.h>
#include "tests.h"
#include <syscall.h>

void printUsage() {
	puts("Usage: tests <mm | prio | sync | process> args");
	exit(-1);
}


int main(int argc, char *argv[]) {
	if (argc < 1) {
		printUsage();
	}
	char const *test = argv[0];
	
	if (strcmp(test, "sync") == 0)
		test_sync(argc - 1, &argv[1]);
	else if (strcmp(test, "process") == 0)
		test_processes(argc - 1, &argv[1]);
	else if (strcmp(test, "prio") == 0)
		test_prio();
	else if (strcmp(test, "mm") == 0)
		test_mm(argc - 1, &argv[1]);
	else {
		printUsage();
	}

	return 0;
}
