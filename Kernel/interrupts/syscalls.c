
#include <stdint.h>
#include "video.h"
#include "syscalls.h"
#include <stdbool.h>
#include "process.h"
#include "interrupts.h"

int64_t read(uint64_t fd, char *buf, uint64_t count) {
    struct ProcessDescriptor process = *getCurrentProcess();
    if (fd == 0)
        return process.fdTable[0].read(process.tty, buf, count);
    return 0;
}


int64_t write(uint64_t fd, char *buf, uint64_t count) {
    struct ProcessDescriptor process = *getCurrentProcess();
    if (fd == 1 || fd == 2)
        return process.fdTable[fd].write(process.tty, buf, count);
    return 0;
}

uint8_t getpid() {
    return getProcessPID(getCurrentProcess());
}

int execve(char *moduleName) {
    struct ProcessDescriptor process = *getCurrentProcess();
    int pid = createProcess(process.tty, moduleName, false);
    if (pid == -1)
        return -1;
    _sti();
    while (getProcess(pid)->active) {
        __asm__("int $0x81");
    }
    _cli();
    return 0;
}

uint64_t proccount() {
    return countProcesses();
}

uint64_t syscallDispatcher(uint64_t rdi, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5) {
    switch (rdi) {
        case READ: // drawChar: char, x, y
            return read(param1, (char *)param2, param3);
        case WRITE: // getChar
            return write(param1, (char *)param2, param3);
        case GETPID: // getChar
            return getpid();
        case EXECVE: // getChar
            return execve((char *)param1);
        case PROCCOUNT: // getChar
            return proccount();
    }
    return 0;
}