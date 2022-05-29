#include "syscall.h"
#include "processes.h"
#include "registers.h"
#include "syscalls.h"
#include <stdint.h>

static uint64_t syscall(uint64_t code, uint64_t param1, uint64_t param2,
                        uint64_t param3, uint64_t param4, uint64_t param5) {
	__asm__ __volatile__("int $0x80");

	register uint64_t ret __asm__("rax");
	return ret;
}

int read(uint64_t fd, char *buf, uint64_t count, uint64_t timeout) {
	return syscall(READ, fd, (uint64_t)buf, count, timeout, 0);
}

int64_t write(uint64_t fd, const char *buf, uint64_t count) {
	return syscall(WRITE, fd, (uint64_t)buf, count, 0, 0);
}

bool dup2(int fd1, int fd2) {
	return syscall(DUP2, fd1, fd2, 0, 0, 0);
}

bool close(int fd) {
	return syscall(CLOSE, fd, 0, 0, 0, 0);
}

uint8_t getpid() { return syscall(GETPID, 0, 0, 0, 0, 0); }

int execve(char *moduleName, char **argv) {
	return syscall(EXECVE, (uint64_t)moduleName, (uint64_t)argv, 0, 0, 0);
}

uint64_t proccount() { return syscall(PROCCOUNT, 0, 0, 0, 0, 0); }

Time gettime() {
	Time time;
	syscall(GETTIME, (uint64_t)&time, 0, 0, 0, 0);
	return time;
}

struct RegistersState getRegisters() {
	struct RegistersState out;
	syscall(GETREGISTERS, (uint64_t)&out, 0, 0, 0, 0);
	return out;
}

int fork() { return syscall(FORK, 0, 0, 0, 0, 0); }

bool exec(char *moduleName, char **args) {
	return syscall(EXEC, (uint64_t)moduleName, (uint64_t)args, 0, 0, 0);
}

int waitpid(int pid) { return syscall(WAITPID, pid, 0, 0, 0, 0); }

bool kill(int pid) { return syscall(KILL, pid, 0, 0, 0, 0); }

void microsleep(uint64_t micros) { syscall(MICROSLEEP, micros, 0, 0, 0, 0); }
void millisleep(uint64_t millis) { microsleep(millis * 1000); }

uint64_t millis() { return syscall(MILLIS, 0, 0, 0, 0, 0); }

uint8_t getProcesses(struct Process processes[256]) {
	return syscall(GETPROCS, (uint64_t)processes, 0, 0, 0, 0);
}

void setGraphic(bool value) { syscall(SETGRAPHIC, value, 0, 0, 0, 0); }

static void drawFigure(enum Figures figure, uint64_t param1, uint64_t param2,
                       uint64_t param3, uint64_t param4) {
	syscall(DRAWFIGURE, figure, param1, param2, param3, param4);
}

void drawCircle(uint16_t x, uint16_t y, uint16_t radius) {
	drawFigure(CIRCLE, x, y, radius, 0);
}
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	drawFigure(RECTANGLE, x, y, width, height);
}
void setForeground(Color color) {
	syscall(SETFOREGROUND, color.red, color.green, color.blue, 0, 0);
}

void drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                Color bitmap[][width]) {
	syscall(DRAWBITMAP, x, y, width, height, (uint64_t)bitmap);
}

void drawText(char *text, uint16_t x, uint16_t y, bool center) {
	drawFigure(TEXT, (uint64_t)text, x, y, center);
}

void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	drawFigure(LINE, x1, y1, x2, y2);
}

WindowInfo getWindowInfo() {
	WindowInfo windowInfo;
	syscall(GETWINDOWINFO, (uint64_t)&windowInfo, 0, 0, 0, 0);
	return windowInfo;
}

void flip() { syscall(FLIP, 0, 0, 0, 0, 0); }

void switchToDesktop(uint8_t desktop) {
	syscall(SWITCHTODESKTOP, desktop, 0, 0, 0, 0);
}

SID semInit(const char *name, semValue value) {
	return syscall(SEMINIT, (uint64_t)name, value, 0, 0, 0);
}

bool semClose(SID sem) { return syscall(SEMCLOSE, sem, 0, 0, 0, 0); }

bool semWait(SID sem) { return syscall(SEMWAIT, sem, 0, 0, 0, 0); }

bool semPost(SID sem) { return syscall(SEMPOST, sem, 0, 0, 0, 0); }

SID semOpen(const char *name) {
	return syscall(SEMOPEN, (uint64_t)name, 0, 0, 0, 0);
}

void semPrintList() { syscall(SEMPRINTLIST, 0, 0, 0, 0, 0); }

bool pipe(int *readFD, int *writeFD) {
	return syscall(PIPE, (uint64_t)readFD, (uint64_t)writeFD, 0, 0, 0);
}

void pipePrintList() { syscall(PIPEPRINTLIST, 0, 0, 0, 0, 0); }

// Allocates 'byteCount' bytes and returns it's memory location.
void *ourMalloc(size_t byteCount) {
	return (void *)syscall(MALLOC, (uint64_t)byteCount, 0, 0, 0, 0);
}

// Frees a previously allocated buffer.
void ourFree(void *memPtr) { syscall(FREE, (uint64_t)memPtr, 0, 0, 0, 0); }

MemoryState getMemoryState() {
	MemoryState state;
	syscall(GETMEMORYSTATE, (uint64_t)&state, 0, 0, 0, 0);
	return state;
}
