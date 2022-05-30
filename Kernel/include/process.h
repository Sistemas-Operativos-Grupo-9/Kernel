#pragma once

#include <pipe.h>
#include <processes.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_PROCESS_COUNT 256
typedef int ID;

struct FileDescriptor {
	bool active;
	bool eof;
	ID id;
	int (*read)(ID id, const char *buf, uint64_t count, uint64_t timeout);
	int (*write)(ID id, const char *buf, uint64_t count);
	void (*dup2)(ID id);
	void (*close)(ID id);
};

#define PROCESS_MEMORY 0x200000
#define MAX_FILE_DESCRIPTORS 256

typedef struct ProcessDescriptor {
	void *stack;
	bool initialized;
	bool toKill;
	bool toFork;
	bool waiting;
	char *name;
	void *entryPoint;
	ProcessState state;
	int returnCode;
	char **args;
	uint8_t priority;
	uint8_t view;
	struct FileDescriptor fdTable[MAX_FILE_DESCRIPTORS];
} ProcessDescriptor;

extern void _startScheduler();
extern void _switchContext();
extern int _yield();
extern void _killAndNextProcess();

void initializeLogProcess();

void exit(int retCode);
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

struct FileDescriptor *createFileDescriptor();
int getFileDescriptorNumber(struct FileDescriptor *fd);

void initializeHaltProcess();

void timerUpdate();
void keypressUpdate();
void childDeadUpdate();
void semaphoreUpdate();

int waitPID(int pid);
void waitForIO();
