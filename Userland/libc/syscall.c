#include "syscall.h"
#include "syscalls.h"


uint64_t syscall(uint64_t code, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5) {
    __asm__("int $0x80");

    register uint64_t ret __asm__("rax");
    return ret;
}


int read(uint64_t fd, char *buf, uint64_t count) {
    return syscall(READ, fd, (uint64_t)buf, count, 0, 0);
}

int write(uint64_t fd, char *buf, uint64_t count) {
    return syscall(WRITE, fd, (uint64_t)buf, count, 0, 0);
}

uint8_t getpid() {
    return syscall(GETPID, 0, 0, 0, 0, 0);
}