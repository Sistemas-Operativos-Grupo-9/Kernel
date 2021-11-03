#include "syscall.h"
#include "syscalls.h"

uint64_t syscall(uint64_t code, uint64_t param1, uint64_t param2,
                 uint64_t param3, uint64_t param4, uint64_t param5) {
	__asm__("int $0x80");

	register uint64_t ret __asm__("rax");
	return ret;
}

int read(uint64_t fd, char *buf, uint64_t count, uint64_t timeout) {
	return syscall(READ, fd, (uint64_t)buf, count, timeout, 0);
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

void microsleep(uint64_t micros) { syscall(MICROSLEEP, micros, 0, 0, 0, 0); }
void millisleep(uint64_t millis) { microsleep(millis * 1000); }

uint64_t millis() { return syscall(MILLIS, 0, 0, 0, 0, 0); }

void setGraphic(bool value) { syscall(SETGRAPHIC, value, 0, 0, 0, 0); }

void drawFigure(enum Figures figure, uint16_t param1, uint16_t param2,
                uint16_t param3, uint16_t param4) {
	syscall(DRAWFIGURE, figure, param1, param2, param3, param4);
}

void drawCircle(uint16_t x, uint16_t y, uint16_t radius) {
	drawFigure(CIRCLE, x, y, radius, 0);
}
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	drawFigure(RECTANGLE, x, y, width, height);
}
void setForeground(uint8_t red, uint8_t green, uint8_t blue) {
	syscall(SETFOREGROUND, red, green, blue, 0, 0);
}

void drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                Color bitmap[][width]) {
	syscall(DRAWBITMAP, x, y, width, height, (uint64_t)bitmap);
}

WindowInfo getWindowInfo() {
	WindowInfo windowInfo;
	syscall(GETWINDOWINFO, (uint64_t)&windowInfo, 0, 0, 0, 0);
	return windowInfo;
}