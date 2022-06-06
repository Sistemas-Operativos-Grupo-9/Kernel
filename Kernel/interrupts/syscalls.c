
#include "syscalls.h"
#include "datetime.h"
#include "gettime.h"
#include "priorityQueue.h"
#include "graphics/video.h"
#include "interrupts.h"
#include "lock.h"
#include "memory_manager.h"
#include "pipe.h"
#include "process.h"
#include "processes.h"
#include "registers.h"
#include "semaphore.h"
#include "time.h"
#include "window.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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

extern int PID;
bool close(int fd) {
	struct ProcessDescriptor *process = getCurrentProcess();
	struct FileDescriptor *d = &process->fdTable[fd];
	if (!d->active)
		return true;
	if (d->close != NULL)
		d->close(d->id);
	d->active = false;
	return false;
}

bool dup2(int fd1, int fd2) {
	if (fd1 == fd2)
		return false;
	struct ProcessDescriptor *process = getCurrentProcess();
	struct FileDescriptor *d1 = &process->fdTable[fd1];
	struct FileDescriptor *d2 = &process->fdTable[fd2];
	if (d2->active) {
		close(fd2);
	}
	if (d1->dup2 != NULL)
		d1->dup2(d1->id);
	*d2 = *d1;
	return false;
}

bool pipe(int *readFD, int *writeFD) {
	PIPID pipe;
	if (pipeInit(&pipe)) {
		return true;
	}
	struct FileDescriptor *readD = createFileDescriptor();
	struct FileDescriptor *writeD = createFileDescriptor();

	if (readD == NULL || writeD == NULL) {
		if (readD != NULL)
			readD->active = false;
		if (writeD != NULL)
			writeD->active = false;
		pipeClose(pipe);
		return true;
	}

	readD->id = pipe;
	readD->write = NULL;
	readD->read = (int (*)(ID, const char *, uint64_t, uint64_t))pipeRead;
	readD->close = (void (*)(ID))pipeDecRead;
	readD->dup2 = (void (*)(ID))pipeIncRead;

	writeD->id = pipe;
	writeD->read = NULL;
	writeD->write = (int (*)(ID, const char *, uint64_t))pipeWrite;
	writeD->close = (void (*)(ID))pipeDecWrite;
	writeD->dup2 = (void (*)(ID))pipeIncWrite;

	*readFD = getFileDescriptorNumber(readD);
	*writeFD = getFileDescriptorNumber(writeD);

	return false;
}

uint8_t getpid() { return getProcessPID(getCurrentProcess()); }

bool setprio(int pid, uint8_t prio) {
	if (prio >= PRIORITY_COUNT)
		return true;
	ProcessDescriptor *process = getProcess(pid);
	if (process->state == PROCESS_DEAD)
		return true;
	process->priority = prio;
	return false;
}

int fork() {
	getCurrentProcess()->toFork = true;
	return _yield();
}

int waitpid(int pid) { return waitPID(pid); }

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

extern struct ProcessDescriptor processes[MAX_PROCESS_COUNT];
uint8_t getProcesses(struct Process processList[MAX_PROCESS_COUNT]) {
	int pi = 0;
	for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
		struct ProcessDescriptor *process = getProcess(i);
		if (process->state != PROCESS_DEAD) {
			processList[pi] = (struct Process){
			    .pid = i,
			    .entryPoint = process->entryPoint,
			    .stackStart = process->entryPoint + PROCESS_MEMORY,
			    .waiting = process->waiting,
			    .state = process->state,
			    .priority = process->priority,
			};
			strcpy(processList[pi].name, process->name);
			pi++;
		}
	}
	return pi;
}

// TODO: choose view for process?.
uint8_t viewForProcess() { return getCurrentProcess()->view; }

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
	case DUP2:
		return dup2(param1, param2);
	case CLOSE:
		return close(param1);
	case EXIT:
		exit(param1);
		break;

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
	case NICE:
		return setprio(param1, param2);
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
	case PIPE:
		return pipe((int *)param1, (int *)param2);
	case PIPEPRINTLIST:
		pipePrintList();
		break;
	case GETMEMORYSTATE:
		getMemoryState((MemoryState *)param1);
		break;
	}
	return 0;
}
