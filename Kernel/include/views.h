#pragma once

#include <stdbool.h>
#include <stdint.h>

uint64_t inputAvailable(uint8_t tty);
void writeChar(uint8_t tty, char ch);
char readInput(uint8_t tty);
void writeOutput(uint8_t tty, char ch);