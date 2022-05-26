#include "processes.h"
#include <print.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>

struct Process processes[256];

int main() {
	puts("There are ");
	int count = getProcesses(processes);
	printUnsigned(count, 10);
	puts(" processes running.\n");
	for (int i = 0; i < count; i++) {
		setForeground(processes[i].waiting ? RED : GREEN);
		puts("[PID=");
		printUnsignedN(processes[i].pid, 3, 10);
		puts("] ");
		puts(processes[i].name);
		putchar(' ');

		// Strange bug when set to <= 10 and trying to print with strlen >= 11
		for (int j = 0; j < 10 - (int)strlen(processes[i].name); j++)
			putchar(' ');
		puts(processes[i].waiting ? "[WAITING]" : "  [READY]");
		switch (processes[i].state) {
			case PROCESS_DEAD:
				puts(" [DEAD]");
				break;
			case PROCESS_ACTIVE:
				puts(" [ACTIVE]");
				break;
			case PROCESS_ZOMBIE:
				puts(" [ZOMBIE]");
				break;
		}
		puts(": ");
		printHexPointer(processes[i].entryPoint);
		puts(" - ");
		printHexPointer(processes[i].stackStart);
		putchar('\n');
	}
	setForeground(WHITE);
	return 0;
}
