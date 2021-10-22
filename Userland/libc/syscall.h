#pragma once

#include <datetime.h>
#include <stdbool.h>
#include <stdint.h>

int read(uint64_t fd, char *buf, uint64_t count, uint64_t timeout);
int write(uint64_t fd, char *buf, uint64_t count);
uint8_t getpid();
int execve(char *moduleName, char **argv, int argc);
uint64_t proccount();
Time gettime();
void printreg();
bool kill(int pid);
void microsleep(uint64_t micros);
void millisleep(uint64_t millis);
uint64_t millis();