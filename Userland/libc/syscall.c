#include "syscall.h"
#include "syscalls.h"

uint64_t syscall(uint64_t code, uint64_t param1, uint64_t param2,
                 uint64_t param3, uint64_t param4, uint64_t param5) {
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

uint8_t getpid() { return syscall(GETPID, 0, 0, 0, 0, 0); }

int execve(char *moduleName, char **argv, int argc) {
	return syscall(EXECVE, (uint64_t)moduleName, (uint64_t)argv, argc, 0, 0);
}

uint64_t proccount() { return syscall(PROCCOUNT, 0, 0, 0, 0, 0); }

Time gettime() {
	Time time;
	syscall(GETTIME, (uint64_t)&time, 0, 0, 0, 0);
	return time;
}

void printreg() { syscall(PRINTREG, 0, 0, 0, 0, 0); }

bool kill(int pid) { return syscall(KILL, pid, 0, 0, 0, 0); }