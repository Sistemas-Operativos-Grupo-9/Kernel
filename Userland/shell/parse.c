#include "parse.h"
#include "string.h"
#include "syscall.h"
#include <stddef.h>

int countPipes(char *command) {
	size_t pipeCount = 0;
	size_t index = 0;

	if (command[index++] == '|')
		return -1;

	while (command[index] != '\0') {
		if (command[index] == '|') {
			pipeCount++;
			// more than one pipe is considered as an error
			if (command[index - 1] == '|')
				return -1;
		}
		index++;
	}
	return pipeCount;
}

bool parseCommands(char *command, struct ParseData *data) {
	// Coutn '|' tokens and check for sintactic errors
	int pipeCount = countPipes(command);
	if (pipeCount == -1) {
		return false;
	}

	// allocate space for the command array
	size_t commandsCount = pipeCount + 1;
	data->commandMatrix = ourMalloc(
	    (commandsCount + 1) * sizeof(char **)); // one extra space for NULL
	if (data->commandMatrix == NULL)
		return false;

	char *splitResult[commandsCount];
	size_t splitSize = split2(command, '|', splitResult);

	if (splitSize != commandsCount)
		return false;

	// cycle through each command and then split it through ' '
	char *currentCommand;
	data->background = false;
	size_t c;
	for (c = 0; c < commandsCount; c++) {
		currentCommand = splitResult[c];

		char *splittedCommand[tokencount(currentCommand, ' ') + 1];
		splitSize = split2(currentCommand, ' ', splittedCommand);

		// check for background command
		if (strcmp(splittedCommand[splitSize - 1], "&") == 0) {
			if (c != commandsCount - 1)
				return false;

			data->background = true;
		}

		data->commandMatrix[c] = ourMalloc(
		    (splitSize + 1) * sizeof(char *)); // one extra space for NULL
		if (data->commandMatrix[c] == NULL)
			return false;

		// save the pointers and add NULL at the end
		size_t a;
		for (a = 0; a < splitSize; a++) {
			data->commandMatrix[c][a] = splittedCommand[a];
		}
		data->commandMatrix[c][a] = NULL;
	}
	data->commandMatrix[c] = NULL;
	return true;
}

void freeParseData(struct ParseData data) {
	size_t commandIndex = 0;
	while (data.commandMatrix[commandIndex] != NULL) {
		ourFree(data.commandMatrix[commandIndex++]);
	}
	ourFree(data.commandMatrix);
}
