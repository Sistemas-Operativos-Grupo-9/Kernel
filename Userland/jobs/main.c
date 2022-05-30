#include "processes.h"
#include <shared-lib/print.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>

struct Process processes[256];

int main() {
	puts("There are ");
	int count = getProcesses(processes);
	printUnsigned(count, 10);
	puts(" processes running.\n");
	for (int i = 1; i < count; i++) {
		struct Process *process = &processes[i];
		setForeground(process->waiting ? RED : GREEN);
		puts("[PID=");
		printUnsignedN(process->pid, 3, 10);
		puts("] ");
		puts("[PRIO=");
		printUnsignedN(process->priority, 1, 10);
		puts("] ");
		puts(process->name);
		putchar(' ');

		// Strange bug when set to <= 10 and trying to print with strlen >= 11
		for (int j = 0; j < 10 - (int)strlen(process->name); j++)
			putchar(' ');
		puts(process->waiting ? "[WAITING]" : "  [READY]");
		switch (process->state) {
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
		printHexPointer(process->entryPoint);
		puts(" - ");
		printHexPointer(process->stackStart);
		putchar('\n');
	}
	setForeground(WHITE);
	return 0;
}
