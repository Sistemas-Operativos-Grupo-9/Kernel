#include "executor.h"
#include "string.h"
#include <shared-lib/print.h>
#include <syscall.h>

#define MAX_BACKGROUND_RUNNING_PROCESSES 256
int backgroundRunningCommands[MAX_BACKGROUND_RUNNING_PROCESSES];
int backgroundRunningCommandCount;

int countToNULL(const void **ptr) {
	int i;
	for (i = 0; ptr[i] != NULL; i++)
		;
	return i;
}

bool tryExecuteBuiltinCommand(const char *command, const char **args) {
	int argc = countToNULL((const void **)args);
	if (strcmp(command, "pid") == 0 && argc == 0) {
		printInt(getpid(), 10);
		return true;
	} else if ((strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) &&
	           argc == 0) {
		exit(0);
		return true;
	}
	return false;
}

void executeCommands(struct ParseData data) {
	int processCount = countToNULL((const void **)data.commandMatrix);

	if (tryExecuteBuiltinCommand(data.commandMatrix[0][0],
	                             (const char **)&data.commandMatrix[0][1])) {
		return;
	}

	SID errorPrintSem = semInit("", 1);
	int processes[processCount];
	int re, we;

	int previousOutput = 0;
	// Give eof to stdin if program runs in background.
	if (data.background) {
		int w;
		pipe(&previousOutput, &w);
		close(w);
	}
	// Execute and connect with pipes all processes.
	for (int i = 0; i < processCount; i++) {
		bool lastCommand = i == processCount - 1;
		bool firstCommand = i == 0;
		int p;
		// Connect last command stdout directly to terminal.
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
				semWait(errorPrintSem);
				eputs("Could not run module \"");
				eputs(data.commandMatrix[i][0]);
				eputs("\". It does not exist.\n");
				semPost(errorPrintSem);
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
	semClose(errorPrintSem);
}

void waitBackgroundProcesses() {
	for (int i = 0; i < backgroundRunningCommandCount; i++) {
		waitpid(backgroundRunningCommands[i]);
	}
}
