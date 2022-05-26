#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <processes.h>

struct FileDescriptor {
	bool eof;
	int (*read)(uint8_t tty, char *buf, uint64_t count, uint64_t timeout);
	int (*write)(uint8_t tty, char *buf, uint64_t count);
};

#define PROCESS_MEMORY 0x200000
#define MAX_FILE_DESCRIPTORS 3


typedef struct __attribute__((packed)) ProcessDescriptor {
	void *stack;
	bool initialized;
	bool toKill;
	bool toFork;
	bool waiting;
	uint8_t tty;
	char *name;
	void *entryPoint;
	ProcessState state;
	int returnCode;
	struct FileDescriptor fdTable[MAX_FILE_DESCRIPTORS];
} ProcessDescriptor;

extern void _startScheduler();
extern void _switchContext();
extern int _yield();
extern void _killAndNextProcess();

bool killProcess(int pid);
void restartProcess();
void terminateProcess();
int getProcessPID(ProcessDescriptor *process);
struct ProcessDescriptor *getCurrentProcess();
struct ProcessDescriptor *getFocusedProcess();
int createProcess(uint8_t tty, char *name, char **argv, int argc,
                  bool restartOnFinish);
void nextProcess();
void setFocus(uint8_t tty);
uint64_t countProcesses();
struct ProcessDescriptor *getProcess(int pid);

void initializeHaltProcess();

void timerUpdate();
void keypressUpdate();
void childDeadUpdate();
void semaphoreUpdate();

int waitPID(int pid);
void waitForIO();
