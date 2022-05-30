#include "stdlib.h"
#include "syscall.h"
#include "shared-lib/print.h"
#include <stdio.h>




void printUsage() {
	eputs("Usage: nice <PID> <NEWPRIO>\n");
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printUsage();
		return -1;
	}

	int64_t pid;
	int64_t prio;
	if (!strtoint(argv[0], (int64_t *)&pid, 10)) {
		eputs("Could not parse pid number.\n");
		printUsage();
		return -2;
	}
	if (!strtoint(argv[1], (int64_t *)&prio, 10)) {
		eputs("Could not parse priority number.\n");
		printUsage();
		return -2;
	}
	printInt(pid, 10);
	printInt(prio, 10);

	if (setprio(pid, prio)) {
		eputs("Error setting priority (inactive PID or priority is not < 3).\n");
		return -3;
	};
	return 0;
}
