#pragma once

#include <stdint.h>
#include <datetime.h>

int read(uint64_t fd, char *buf, uint64_t count);
int write(uint64_t fd, char *buf, uint64_t count);
uint8_t getpid();
int execve(char *moduleName, char **argv, int argc);
uint64_t proccount();
Time gettime();
void printreg();