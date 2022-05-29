#include "executor.h"
#include "input.h"
#include "parse.h"
#include <shared-lib/print.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <time.h>

// bool validateCommand(char *command) {
// 	if (strlen(command) > 0) {
// 		return true;
// 	}
// 	return false;
// }

// // Returns the arg count
// void splitArguments(char *command, char **splitted) {
// 	int count = 0;
// 	char *start = command;
// 	bool out = false;
// 	do {
// 		out = *command == '\0';
// 		if (*command == ' ' || out) {
// 			*command = '\0';
// 			splitted[count] = start;
// 			start = command + 1;
// 			count++;
// 		}
// 	} while (command++, !out);
// 	splitted[count] = NULL;
// }

// // Returns true if exit
// bool execCommand(char *command) {
// 	char *exec;
// 	char *argv[10];
// 	splitArguments(command, argv);
// 	exec = argv[0];
// 	if (strcmp(exec, "quit") == 0 || strcmp(exec, "exit") == 0) {
// 		return true;
// 	}
// 	if (strcmp(exec, "pid") == 0) {
// 		printInt(getpid(), 10);
// 	} else if (execve(exec, argv + 1) == -1) {
// 		putchar('\"');
// 		puts(exec);
// 		puts("\" is not a recognized program or module\n");
// 	}

// 	// puts(process->name);
// 	// puts(" -> ");
// 	// printInt(retCode, 10);
// 	// putchar('\n');

// 	setForeground(WHITE);
// 	return false;
// }

int main(int argc, char **argv) {
	if (argc == 1 && strcmp(argv[0], "--print-help") == 0) {
		puts("Use PgUp and PgDown to scroll.\n");
		puts("View command history with up and down arrows.\n");
		puts("Type \"help\" to see all the programs and commands.\n");
	}
	char command[MAX_COMMAND_SIZE];
	while (true) {
		puts("\n~");
		bool eof = inputCommand(command, sizeof(command));
		if (eof)
			break;
		/*puts(command);*/
		char commandCopy[MAX_COMMAND_SIZE];
		strncpy(commandCopy, command, MAX_COMMAND_SIZE);
		struct ParseData data;
		if (parseCommands(command, &data)) {
			/*for (int i = 0; data.commandMatrix[i] != NULL; i++) {*/
			/*for (int j = 0; data.commandMatrix[i][j] != NULL; j++) {*/
			/*putchar(' ');*/
			/*puts(data.commandMatrix[i][j]);*/
			/*}*/
			/*puts("|\n");*/
			/*}*/
			saveCommand(commandCopy);
			putchar('\n');
			executeCommands(data);
		}
	}

	waitBackgroundProcesses();
	printInt(getpid(), 10);

	return 0;
}
