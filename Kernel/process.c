
#include <views.h>
#include "process.h"
#include "video.h"
#include "queue.h"
#include "null.h"

#define NO_PID -1

static struct ProcessDescriptor processes[10];
extern bool schedulerEnabled;
bool schedulerEnabled = false;
extern void * ret00;

Queue readyQueue;

extern int PID;
int PID = NO_PID;
static int lastPID = 0;

static int focusPID = 0;

struct ProcessDescriptor *getCurrentProcess() {
    if (PID == NO_PID)
        return NULL;
    return &processes[PID];
}

struct ProcessDescriptor *getFocusedProcess() {
    if (focusPID == NO_PID)
        return NULL;
    return &processes[focusPID];
}

uint64_t readTTY(uint8_t tty, char *buf, uint64_t count) {
    uint64_t read;
    for (read = 0; read < count && buf[read] != '\n'; read++) {
        while (inputAvailable(tty) == 0)
            __asm__ ("int $0x20");// && !isEOF(tty));
        // if (inputAvailable(tty) == 0)
        //     break;
        buf[read] = readInput(tty);
    }
    return read;
}
uint64_t writeTTY(uint8_t tty, char *buf, uint64_t count) {
    for (int i = 0; i < count; i++)
        writeOutput(tty, buf[i]);
    return count;
}

uint64_t createProcess(uint8_t tty, uint64_t *start, uint64_t *stack) {
    uint64_t *stackInit = stack;
    *stack = (uint64_t)start;
    *--stack = 0;
    *--stack = (uint64_t)stackInit;
    *--stack = 0x202;
    *--stack = 8;
    *--stack = (uint64_t)start;
    *--stack = (uint64_t)&ret00;
    struct ProcessDescriptor process = (struct ProcessDescriptor) {
        .tty = tty,
        .fdTable = {
            (struct FileDescriptor) {.read = (int (*)(uint8_t, char *, uint64_t))readTTY},
            (struct FileDescriptor) {.write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
            (struct FileDescriptor) {.write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
        },
        .active = true,
        .stack = stack
    };
    uint64_t pid = lastPID++;
    processes[pid] = process;
    enqueueItem(&readyQueue, &processes[pid]);
    return pid;
}

int getProcessPID(ProcessDescriptor *process) {
    return process - processes;
}

void setFocus(uint8_t tty) {
    changeFocusView(tty);
    for (int i = 0; i < sizeof(processes) / sizeof(*processes); i++) {
        if (processes[i].active && processes[i].tty == tty) {
            focusPID = i;
            return;
        }
    }
    focusPID = -1;
}
