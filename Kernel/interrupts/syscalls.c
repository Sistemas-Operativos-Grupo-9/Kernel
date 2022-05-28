
#include "syscalls.h"
#include "datetime.h"
#include "gettime.h"
#include "graphics/video.h"
#include "interrupts.h"
#include "lock.h"
#include "process.h"
#include "processes.h"
#include "registers.h"
#include "semaphore.h"
#include "time.h"
#include "window.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "memory_manager.h"

int64_t read(uint64_t fd, char *buf, uint64_t count, uint64_t timeout) {
	struct ProcessDescriptor process = *getCurrentProcess();
	struct FileDescriptor d = process.fdTable[fd];
	return d.read(d.id, buf, count, timeout);
}

int64_t write(uint64_t fd, const char *buf, uint64_t count) {
	struct ProcessDescriptor process = *getCurrentProcess();
	struct FileDescriptor d = process.fdTable[fd];
	return d.write(d.id, buf, count);
}

uint8_t getpid() { return getProcessPID(getCurrentProcess()); }

int fork() {
	getCurrentProcess()->toFork = true;
	return _yield();
}

int waitpid(int pid) { return waitPID(pid); }

// TODO: remove execve
int execve(char *moduleName, char **argv) {
	/*struct ProcessDescriptor *process = getCurrentProcess();*/
	int pid = createProcess(focusedView, moduleName, argv);
	if (pid == -1)
		return -1;
	_sti();
	while (getProcess(pid)->state == PROCESS_ACTIVE) {
		waitForIO();
	}
	waitPID(pid);
	_cli();
	return 0;
}

uint64_t proccount() { return countProcesses(); }

void gettime(Time *time) { *time = getTime(); }

struct RegistersState registersState = {0};

void getRegisters(struct RegistersState *out) { *out = registersState; }

// extern void _printRegisters();
// void printreg() { _printRegisters(); }

bool kill(int pid) { return killProcess(pid); }

void microsleep(uint64_t micros) {
	uint64_t start = microseconds_elapsed();
	while (microseconds_elapsed() - start < micros) {
		waitForIO();
	}
}

uint64_t millis() { return microseconds_elapsed() / 1000; }

extern struct ProcessDescriptor processes[256];
uint8_t getProcesses(struct Process processList[256]) {
	int pi = 0;
	for (int i = 0; i < 256; i++) {
		struct ProcessDescriptor *process = getProcess(i);
		if (process->state != PROCESS_DEAD) {
			processList[pi] = (struct Process){
			    .pid = i,
			    .entryPoint = process->entryPoint,
			    .stackStart = process->entryPoint + PROCESS_MEMORY,
			    .waiting = process->waiting,
			    .state = process->state,
			};
			strcpy(processList[pi].name, process->name);
			pi++;
		}
	}
	return pi;
}

// TODO: choose view for process?.
uint8_t viewForProcess() { return 0; }

void setGraphic(bool value) { setViewGraphic(viewForProcess(), value); }

void drawCircleCall(uint16_t x, uint16_t y, uint16_t radius) {
	drawCircle(viewForProcess(), x, y, radius);
}

void drawRectangleCall(uint16_t x, uint16_t y, uint16_t width,
                       uint16_t height) {
	drawRectangle(viewForProcess(), x, y, width, height);
}

void drawTextCall(char *text, uint16_t x, uint16_t y, bool center) {
	drawText(viewForProcess(), text, x, y, center);
}

void drawLineCall(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	drawLine(viewForProcess(), x1, y1, x2, y2);
}

void drawFigure(enum Figures figure, uint64_t param2, uint64_t param3,
                uint64_t param4, uint64_t param5) {
	switch (figure) {
	case CIRCLE:
		drawCircleCall(param2, param3, param4);
		break;
	case RECTANGLE:
		drawRectangleCall(param2, param3, param4, param5);
		break;
	case TEXT:
		drawTextCall((char *)param2, param3, param4, param5);
		break;
	case LINE:
		drawLineCall(param2, param3, param4, param5);
		break;
	}
}

void drawBitmapCall(uint16_t x, uint16_t y, const uint16_t width,
                    const uint16_t height, Color bitmap[][width]) {
	drawBitmap(viewForProcess(), x, y, width, height, bitmap);
}

void setForegroundCall(uint8_t red, uint8_t green, uint8_t blue) {
	setForeground(viewForProcess(), (Color){
	                                    .red = red,
	                                    .green = green,
	                                    .blue = blue,
	                                });
}

void getWindowInfo(WindowInfo *windowInfo) {
	getViewInfo(viewForProcess(), windowInfo);
}

void flipCall() { flip(viewForProcess()); }

void switchToDesktop(uint8_t desktop) { focusDesktop(desktop); }

uint64_t syscallDispatcher(uint64_t rdi, uint64_t param1, uint64_t param2,
                           uint64_t param3, uint64_t param4, uint64_t param5) {
	switch (rdi) {
	case READ: // drawChar: char, x, y
		return read(param1, (char *)param2, param3, param4);
	case WRITE: // getChar
		return write(param1, (const char *)param2, param3);

	case SEMPOST:
		return semPost(param1);
	case SEMWAIT:
		return semWait(param1);
	case SEMINIT:
		return semInit((char *)param1, param2);
	case SEMOPEN:
		return semOpen((char *)param1);
	case SEMCLOSE:
		return semClose(param1);
	case SEMPRINTLIST:
		semPrintList();
		break;

	case GETPID:
		return getpid();
	case EXECVE:
		return execve((char *)param1, (char **)param2);
	case FORK:
		return fork();
	case EXEC:
		return exec((char *)param1, (char **)param2);
	case WAITPID:
		return waitpid((int)param1);
	case PROCCOUNT:
		return proccount();
	case GETTIME:
		gettime((Time *)param1);
		break;
	case GETREGISTERS:
		getRegisters((struct RegistersState *)param1);
		break;
	case KILL:
		return kill(param1);
	case MICROSLEEP:
		microsleep(param1);
		break;
	case MILLIS:
		return millis();
	case GETPROCS:
		return getProcesses((struct Process *)param1);
	case SETGRAPHIC:
		setGraphic(param1);
		break;
	case DRAWFIGURE:
		drawFigure(param1, param2, param3, param4, param5);
		break;
	case SETFOREGROUND:
		setForegroundCall(param1, param2, param3);
		break;
	case GETWINDOWINFO:
		getWindowInfo((WindowInfo *)param1);
		break;
	case DRAWBITMAP:
		drawBitmapCall(param1, param2, param3, param4, (Color(*)[])param5);
		break;
	case FLIP:
		flipCall();
		break;
	case SWITCHTODESKTOP:
		switchToDesktop(param1);
		break;
	case MALLOC:
		return (uint64_t)ourMalloc(param1);
	case FREE:
		ourFree((void *)param1);
		break;
	case GETMEMORYSTATE:
		getMemoryState((MemoryState *)param1);
		break;
	}
	return 0;
}
