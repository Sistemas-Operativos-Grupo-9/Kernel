
#include <views.h>
#include "process.h"
#include "video.h"

static struct ProcessDescriptor processes[10];

static int PID = 0;
static int lastPID = 0;

static int focusPID = 0;

struct ProcessDescriptor getCurrentProcess() {
    return processes[PID];
}

struct ProcessDescriptor getFocusedProcess() {
    return processes[focusPID];
}

uint64_t readTTY(uint8_t tty, char *buf, uint64_t count) {
    uint64_t read;
    for (read = 0; read < count && buf[read] != '\n'; read++) {
        // while (getAvailable(tty) == 0 && !isEOF(tty));
        if (inputAvailable(tty) == 0)
            break;
        buf[read] = readInput(tty);
    }
    return read;
}
uint64_t writeTTY(uint8_t tty, char *buf, uint64_t count) {
    for (int i = 0; i < count; i++)
        writeOutput(tty, buf[i]);
    return count;
}

struct ProcessDescriptor createProcess(uint8_t tty) {
    struct ProcessDescriptor process = (struct ProcessDescriptor) {
        .tty = tty,
        .fdTable = {
            (struct FileDescriptor) {.read = (int (*)(uint8_t, char *, uint64_t))readTTY},
            (struct FileDescriptor) {.write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
            (struct FileDescriptor) {.write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
        }
    };
    processes[lastPID++] = process;
    return process;
}

void setFocus(uint8_t tty) {
    focusPID = tty;
    changeFocusView(focusPID);
}

void nextProcess() {
    PID++;
    PID %= 2;
}