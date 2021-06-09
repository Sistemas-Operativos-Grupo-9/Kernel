#pragma once

#include <stdint.h>
#include <stdbool.h>

struct FileDescriptor {
    bool eof;
    int (*read)(uint8_t tty, char *buf, uint64_t count);
    int (*write)(uint8_t tty, char *buf, uint64_t count);
};

struct ProcessDescriptor
{
    uint8_t tty;
    struct FileDescriptor fdTable[3];
};

struct ProcessDescriptor getCurrentProcess();
struct ProcessDescriptor createProcess(uint8_t tty);
void nextProcess();
struct ProcessDescriptor getFocusedProcess();
void setFocus(uint8_t tty);
