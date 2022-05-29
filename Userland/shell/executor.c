#include "executor.h"
#include <shared-lib/print.h>
#include <syscall.h>

#define MAX_BACKGROUND_RUNNING_PROCESSES 256
int backgroundRunningCommands[MAX_BACKGROUND_RUNNING_PROCESSES];
int backgroundRunningCommandCount;

int countToNULL(void **ptr) {
	int i;
	for (i = 0; ptr[i] != NULL; i++)
		;
	return i;
}

void executeCommands(struct ParseData data) {
	int processCount = countToNULL((void **)data.commandMatrix);

	int processes[processCount];
	int re, we;

	int previousOutput = 0;
	for (int i = 0; i < processCount; i++) {
		bool lastCommand = i == processCount - 1;
		bool firstCommand = i == 0;
		int p;
		if (lastCommand)
			we = 1;
		else
			pipe(&re, &we);

		switch (p = fork()) {
		case 0:
			dup2(previousOutput, 0);
			dup2(we, 1);
			if (!lastCommand) {
				close(we);
				close(re);
			}
			if (!firstCommand)
				close(previousOutput);
			if (!exec(data.commandMatrix[i][0], &data.commandMatrix[i][1])) {
				puts("Error");
				exit(1);
			}
		default:
			break;
		}

		if (!firstCommand) {
			close(previousOutput);
		}
		if (!lastCommand) {
			close(we);
			previousOutput = re;
		}
		processes[i] = p;
	}

	if (!data.background) {
		for (int i = 0; i < processCount; i++) {
			waitpid(processes[i]);
		}
	} else {
		for (int i = 0; i < processCount; i++) {
			backgroundRunningCommands[backgroundRunningCommandCount++] =
			    processes[i];
		}
	}
}

void waitBackgroundProcesses() {
	for (int i = 0; i < backgroundRunningCommandCount; i++) {
		waitpid(backgroundRunningCommands[i]);
	}
}