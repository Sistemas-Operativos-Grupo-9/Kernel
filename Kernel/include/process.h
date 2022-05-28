#pragma once

#include <pipe.h>
#include <processes.h>
#include <stdbool.h>
#include <stdint.h>

typedef int ID;

struct FileDescriptor {
	bool eof;
	ID id;
	int (*read)(ID id, const char *buf, uint64_t count, uint64_t timeout);
	int (*write)(ID id, const char *buf, uint64_t count);
};

#define PROCESS_MEMORY 0x200000
#define MAX_FILE_DESCRIPTORS 3

typedef struct __attribute__((packed)) ProcessDescriptor {
	void *stack;
	bool initialized;
	bool toKill;
	bool toFork;
	bool waiting;
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

void initializeLogProcess();
bool killProcess(int pid);
void restartProcess();
void terminateProcess();
int getProcessPID(ProcessDescriptor *process);
struct ProcessDescriptor *getCurrentProcess();
struct ProcessDescriptor *getFocusedProcess();
int createProcess(uint8_t tty, char *name, char **argv);
void nextProcess();
void setFocus(uint8_t tty);
uint64_t countProcesses();
struct ProcessDescriptor *getProcess(int pid);
bool exec(char *moduleName, char **args);

void initializeHaltProcess();

void timerUpdate();
void keypressUpdate();
void childDeadUpdate();
void semaphoreUpdate();

int waitPID(int pid);
void waitForIO();
