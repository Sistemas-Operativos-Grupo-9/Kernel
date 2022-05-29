#pragma once
#include <stdbool.h>
#include <stddef.h>

struct ParseData {
	char ***commandMatrix;
	bool background;
};

// returns true on success, false on error
bool parseCommands(char *command, struct ParseData *data);

void freeParseData(struct ParseData data);