#pragma once
#include <stdint.h>


int read(uint64_t fd, char *buf, uint64_t count);
int write(uint64_t fd, char *buf, uint64_t count);
uint8_t getpid();