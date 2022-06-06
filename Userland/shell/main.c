#include "executor.h"
#include "input.h"
#include "parse.h"
#include <shared-lib/print.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <time.h>

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

	return 0;
}
