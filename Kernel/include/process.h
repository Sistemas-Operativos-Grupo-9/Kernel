#pragma once

#include <stdint.h>
#include <stdbool.h>

struct FileDescriptor {
    bool eof;
    int (*read)(uint8_t tty, char *buf, uint64_t count);
    int (*write)(uint8_t tty, char *buf, uint64_t count);
};

typedef struct __attribute__((packed)) ProcessDescriptor
{
    void *stack;
    bool initialized;
    uint8_t tty;
    bool active;
    struct FileDescriptor fdTable[3];
} ProcessDescriptor;

extern void _startScheduler();
extern void _nextProcess();

struct ProcessDescriptor *getCurrentProcess();
struct ProcessDescriptor *getFocusedProcess();
uint64_t createProcess(uint8_t tty, uint64_t *start, uint64_t *stack);
void nextProcess();
void setFocus(uint8_t tty);