
#include <views.h>
#include "process.h"
#include "video.h"
#include "queue.h"
#include "null.h"
#include "interrupts/interrupts.h"
#include "lock.h"
#define NO_PID -1

static struct ProcessDescriptor processes[256];
extern bool schedulerEnabled;
bool schedulerEnabled = false;
extern void * ret00;

Queue readyQueue;

extern int PID;
int PID = NO_PID;
static int lastPID = -1;

static int focusPID = 0;

static int getFreePID() {
    lastPID++;

    if (lastPID >= sizeof(processes) / sizeof(*processes)) {
        lastPID = 0;
    }
    while (processes[lastPID].active)
        lastPID++;

    return lastPID;
}

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

extern void _nextProcess();
uint64_t readTTY(uint8_t tty, char *buf, uint64_t count) {
    uint64_t read;
    _sti();
    for (read = 0; read < count && buf[read] != '\n'; read++) {
        while (inputAvailable(tty) == 0) {
            // _nextProcess();
            __asm__("int $0x81");

            // _nextProcess();
        }
            // ;
            // __asm__ ("int $0x20");// && !isEOF(tty));
        // if (inputAvailable(tty) == 0)
        //     break;
        buf[read] = readInput(tty);
    }
    _cli();
    return read;
}
uint64_t writeTTY(uint8_t tty, char *buf, uint64_t count) {
    for (int i = 0; i < count; i++)
        writeOutput(tty, buf[i]);
    return count;
}

void restartProcess() {
    _cli();
    ProcessDescriptor *process = getCurrentProcess();
    process->active = false;
    register int retCode __asm__("eax");
    print(0, "Process ");
    print(0, process->name);
    print(0, " terminated with code: ");
    printInt(0, retCode, 16);
    printChar(0, '\n');
    // createProcess(process->tty, process->name, process->entryPoint, process->initialStack);
    createProcess(process->tty, process->name, process->entryPoint, process->initialStack - 0x200, true);
    // _sti();
    _killAndNextProcess();
}

uint64_t createProcess(uint8_t tty, char *name, uint64_t *entryPoint, uint64_t *stack, bool restartOnFinish) {
    uint64_t *stackInit = stack;
    *stack = (uint64_t)restartProcess;
    *--stack = (uint64_t)entryPoint;
    *--stack = 0;
    *--stack = (uint64_t)stackInit;
    *--stack = 0x202;
    *--stack = 8;
    *--stack = (uint64_t)entryPoint;
    *--stack = 0; // This value is changed in _switchContext
    uint64_t pid = getFreePID();
    processes[pid] = (struct ProcessDescriptor) {
        .tty = tty,
        .name = name,
        .fdTable = {
            (struct FileDescriptor) {.read = (int (*)(uint8_t, char *, uint64_t))readTTY},
            (struct FileDescriptor) {.write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
            (struct FileDescriptor) {.write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
        },
        .active = true,
        .stack = stack,
        .initialStack = stackInit,
        .entryPoint = entryPoint
    };
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
