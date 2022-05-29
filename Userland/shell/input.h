#pragma once
#include <stdbool.h>

#define MAX_COMMAND_SIZE 256

bool inputCommand(char *buffer, int maxLength);
void saveCommand(char *command);