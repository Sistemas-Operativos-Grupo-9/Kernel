#include "process.h"
#include "graphics/video.h"
#include "interrupts.h"
#include "lib.h"
#include "lock.h"
#include "memory_manager.h"
#include "moduleLoader.h"
#include "priorityQueue.h"
#include "queue.h"
#include "time.h"
#include <graphics/views.h>
#include <stddef.h>
#include <stdint.h>
#define NO_PID -1
#define HALT_PID 0

static struct ProcessDescriptor processes[MAX_PROCESS_COUNT];
static uint64_t maxProcessCount = sizeof(processes) / sizeof(*processes);
extern bool schedulerEnabled;
bool schedulerEnabled = false;
extern void *ret00;

PriorityQueue readyQueue;
Queue waitingQueue;

extern int PID;
int PID = 0;

static int getFreePID() {
	static int lastPID = 0;
	int startPid = lastPID;

	while (processes[lastPID].state != PROCESS_DEAD) {
		lastPID++;
		if (lastPID >= sizeof(processes) / sizeof(*processes)) {
			lastPID = 0;
		}
		if (lastPID == startPid) {
			return NO_PID;
		}
	}

	return lastPID;
}

struct FileDescriptor *createFileDescriptor() {
	ProcessDescriptor *process = getProcess(PID);
	for (int i = 0; i < MAX_FILE_DESCRIPTORS; i++) {
		struct FileDescriptor *fd = &process->fdTable[i];
		if (!fd->active) {
			fd->active = true;
			return fd;
		}
	}
	return NULL;
}

int getFileDescriptorNumber(struct FileDescriptor *fd) {
	if (fd == NULL)
		return -1;
	ProcessDescriptor *process = getProcess(PID);
	return fd - process->fdTable;
}

struct ProcessDescriptor *getCurrentProcess() {
	if (PID == NO_PID)
		return NULL;
	return &processes[PID];
}

/*struct ProcessDescriptor *getFocusedProcess() {*/
/*if (focusPID == NO_PID)*/
/*return NULL;*/
/*return &processes[focusPID];*/
/*}*/

// Wakes up every process in the waiting list.
// The ones that did not expect to get woken up by this event will go back to
// the waiting queue by their own
void unpauseProcesses() {
	startLock();
	int size = getLength(&waitingQueue);
	for (int i = 0; i < size; i++) {
		ProcessDescriptor *process = dequeueItem(&waitingQueue);
		if (process->blocked) {
			enqueueItem(&waitingQueue, process);
		} else {
			process->waiting = false;
			enqueueItemInPriority(&readyQueue, process, process->priority);
		}
	}
	endLock();
}

void unpauseSomeProcesses(Queue *processQueue) {
	startLock();
	while (getLength(processQueue) > 0) {
		ProcessDescriptor *process = dequeueItem(processQueue);
		process->waiting = false;
		process->blockedOnSem = false;
		enqueueItemInPriority(&readyQueue, process, process->priority);
	}
	endLock();
}

void timerUpdate() { unpauseProcesses(); }

void keypressUpdate() { unpauseProcesses(); }

void childDeadUpdate() { unpauseProcesses(); }

extern void _nextProcess();
int64_t readTTY(uint8_t tty, char *buf, uint64_t count, uint64_t timeout) {
	uint64_t start = microseconds_elapsed() / 1000;
	uint64_t read;
	if (hasEof(tty)) {
		setEof(tty, false);
		return -1;
	}
	_sti();
	for (read = 0; read < count && (read == 0 || buf[read - 1] != '\n');
	     read++) {
		while (
		    inputAvailable(tty) == 0 &&
		    (timeout == 0 || microseconds_elapsed() / 1000 < start + timeout)) {
			// _nextProcess();
			wait();
			if (hasEof(tty)) {
				return read;
			}

			// _nextProcess();
		}
		if (timeout > 0 && microseconds_elapsed() / 1000 >= start + timeout) {
			return read;
		}
		// ;
		// __asm__ ("int $0x20");// && !isEOF(tty));
		// if (inputAvailable(tty) == 0)
		//     break;
		buf[read] = readInput(tty);
	}
	_cli();
	return read;
}
uint64_t writeTTY(int tty, char *buf, uint64_t count) {
	for (int i = 0; i < count; i++)
		writeOutput(tty, buf[i]);
	return count;
}

void wait() {
	getCurrentProcess()->waiting = true;
	_yield();
}

void exit(int retCode) {
	ProcessDescriptor *process = getProcess(PID);
	process->returnCode = retCode;

	childDeadUpdate();
	terminateProcess();

	// ¯\_(ツ)_/¯
	exit(retCode);
}

bool killProcess(int pid) {
	ProcessDescriptor *process = getProcess(pid);
	if (process->state == PROCESS_DEAD)
		return false;
	process->toKill = true;
	return true;
}

bool setBlock(int pid, bool block) {
	ProcessDescriptor *process = getProcess(pid);
	if (process->state == PROCESS_DEAD)
		return false;
	process->blocked = block;
	return true;
}

int waitPID(int pid) {
	ProcessDescriptor *process = getProcess(pid);
	while (process->state != PROCESS_ZOMBIE) {
		wait();
	}
	int ret = process->returnCode;
	process->state = PROCESS_DEAD;
	return ret;
}

void initializeLogProcess() {
	processes[0].fdTable[1] = (struct FileDescriptor){
	    .active = true,
	    .write = (int (*)(ID, const char *, uint64_t))writeTTY,
	    .id = 0,
	};
}

void freeArgs() {
	ProcessDescriptor *process = getCurrentProcess();
	for (int i = 0; process->args[i] != NULL; i++) {
		ourFree(process->args[i]);
	}
	ourFree(process->args);
}

bool close(int fd);
void terminateProcess() {
	ProcessDescriptor *process = getCurrentProcess();
	close(0);
	close(1);
	close(2);
	process->state = PROCESS_ZOMBIE;

	freeArgs();

	_killAndNextProcess();
	__asm__ __volatile__("add $8, %rsp");
	__asm__ __volatile__("iretq");
}

void processReturned() {
	int retCode;
	__asm__ __volatile__("movq %%rax, %q0" : "=r"(retCode)::"rax");
	_cli();
	ProcessDescriptor *process = getCurrentProcess();
	process->returnCode = retCode;

	childDeadUpdate();
	terminateProcess();
}

void haltMain() {
	while (true) {
		__asm__ __volatile__("hlt");
	}
}

static uint64_t haltStack[512] = {0};
extern uint64_t *iretq;

void initializeHaltProcess() {

	uint64_t *stack = (uint64_t *)(&haltStack[512 - 1]);
	uint64_t *stackInit = stack;
	*stack = (uint64_t)processReturned;
	*--stack = (uint64_t)haltMain;
	*--stack = 0;
	*--stack = (uint64_t)stackInit;
	*--stack = 0x202;
	*--stack = 8;
	*--stack = (uint64_t)haltMain;
	*--stack = 0;
	*--stack = 0; // Stack return address (changed in _switchContext)
	*--stack = (uint64_t)0;
	*--stack = (uint64_t)0;
	for (int i = 0; i < 15; i++)
		*--stack = 0;
	// stack -= 14; // 16 - 2

	struct ProcessDescriptor haltProcess = {
	    .name = "halt",
	    .fdTable = {},
	    .state = PROCESS_ACTIVE,
	    .stack = stack,
	};
	processes[HALT_PID] = haltProcess;
}
extern uint8_t endOfKernelStack;

#define PROCESSES_START &endOfKernelStack

void getProcessBoundaries(int pid, void **processStart, void **processEnd) {
	*processStart = (void *)((pid - 1) * PROCESS_MEMORY + PROCESSES_START);
	*processEnd = *processStart + PROCESS_MEMORY - 16;
}

uint64_t countToNull(char **values) {
	if (values == NULL)
		return 0;
	uint64_t i;
	for (i = 0; values[i] != NULL; i++)
		;
	return i;
}

char **copyArgs(char **args) {
	if (args == NULL) return NULL;
	int argc = countToNull(args);
	char **argscopy = ourMalloc((argc + 1) * sizeof(char *));
	for (int i = 0; args[i] != NULL; i++) {
		int arglen = strlen(args[i]);
		argscopy[i] = ourMalloc(arglen + 1);
		strcpy(argscopy[i], args[i]);
	}
	argscopy[argc] = NULL;
	return argscopy;
}

void initializeProcessStack(uint64_t **stackStart, void *entryPoint,
                            char **args) {
	uint64_t *stack = *stackStart;
	uint64_t *stackInit = stack;
	*stack = (uint64_t)processReturned;
	*--stack = (uint64_t)entryPoint;
	*--stack = 0;
	*--stack = (uint64_t)stackInit;
	*--stack = 0x202;
	*--stack = 8;
	*--stack = (uint64_t)entryPoint;
	*--stack = 0;
	*--stack = 0; // Stack return address (changed in _switchContext)
	*--stack = countToNull(args);
	*--stack = (uint64_t)args;
	for (int i = 0; i < 15; i++)
		*--stack = 0;
	// stack -= 14; // 16 - 2
	*stackStart = stack;
}

int createProcess(uint8_t tty, char *name, char **args) {
	struct Module *module = getModule(name);
	if (module == NULL)
		return -1;
	startLock();
	uint64_t pid = getFreePID();
	void *entryPoint;
	uint64_t *stack;
	getProcessBoundaries(pid, &entryPoint, (void **)&stack);

	memcpy((void *)entryPoint, (void *)module->address, module->size);

	char **argscopy = copyArgs(args);
	initializeProcessStack(&stack, entryPoint, argscopy);

	ProcessDescriptor *process = &processes[pid];
	*process = (struct ProcessDescriptor){
	    .name = module->name,
	    .entryPoint = entryPoint,
	    .fdTable =
	        {
	            (struct FileDescriptor){
	                .active = true,
	                .read =
	                    (int (*)(ID, const char *, uint64_t, uint64_t))readTTY,
	                .id = tty,
	            },
	            (struct FileDescriptor){
	                .active = true,
	                .write = (int (*)(ID, const char *, uint64_t))writeTTY,
	                .id = tty,
	            },
	            (struct FileDescriptor){
	                .active = true,
	                .write = (int (*)(ID, const char *, uint64_t))writeTTY,
	                .id = tty,
	            },
	        },
		.view = tty,
	    .state = PROCESS_ACTIVE,
	    .stack = stack,
	    .args = argscopy,
		.priority = 1,
	};
	enqueueItemInPriority(&readyQueue, process, process->priority);
	endLock();
	return pid;
}

bool exec(char *moduleName, char **args) {
	struct Module *module = getModule(moduleName);
	if (module == NULL)
		return false;

	int pid = PID;
	void *entryPoint;
	uint64_t *stack;
	getProcessBoundaries(pid, &entryPoint, (void **)&stack);

	memcpy((void *)entryPoint, (void *)module->address, module->size);

	char **argscopy = copyArgs(args);
	initializeProcessStack(&stack, entryPoint, argscopy);

	ProcessDescriptor *process = getProcess(pid);
	process->name = module->name;
	process->state = PROCESS_ACTIVE;
	process->stack = stack;
	process->args = argscopy;
	enqueueItemInPriority(&readyQueue, process, process->priority);

	_killAndNextProcess();
	__asm__ __volatile__("add $8, %rsp");
	__asm__ __volatile__("iretq");

	return true;
}

int getProcessPID(ProcessDescriptor *process) { return process - processes; }

// Returns new process pid
int cloneProcess(int pid) {
	void *start, *end;
	void *dstStart, *dstEnd;
	ProcessDescriptor *process = getProcess(pid);
	getProcessBoundaries(pid, &start, &end);
	int newPid = getFreePID();
	if (newPid == NO_PID) {
		((uint64_t *)process->stack)[13] = (int)-1;
		return -1;
	}
	getProcessBoundaries(newPid, &dstStart, &dstEnd);
	memcpy(dstStart, start, (uint64_t)end - (uint64_t)start + 1 * 8);
	ProcessDescriptor *newProcess = getProcess(newPid);
	*newProcess = *process;
	// Fix pipes not counting forked process pipe ends.
	for (int i = 0; i < MAX_FILE_DESCRIPTORS; i++) {
		struct FileDescriptor *fd = &process->fdTable[i];
		if (fd->active && fd->dup2 != NULL)
			fd->dup2(fd->id);
	}
	int64_t separation = dstStart - start;
	newProcess->stack = process->stack + separation;
	newProcess->entryPoint = dstStart;
	((uint64_t *)newProcess->stack)[13] = 0;
	((uint64_t *)process->stack)[13] = newPid;

	// 👀
	for (uint64_t *i = dstStart; (void *)i < dstEnd; i++) {
		if (*i >= (uint64_t)start && *i < (uint64_t)end) {
			*i += separation;
		}
	}

	enqueueItemInPriority(&readyQueue, newProcess, newProcess->priority);
	return newPid;
}

void *doSwitch(bool pushCurrent, uint64_t *stackPointer) {
	if (pushCurrent) {
		ProcessDescriptor *process = getCurrentProcess();
		process->initialized = true;
		process->stack = stackPointer;
		if (process->toKill) {
			process->toKill = false;
			endLock();
			freeArgs();
			process->returnCode = -127;
			close(0);
			close(1);
			close(2);
			process->state = PROCESS_ZOMBIE;
			childDeadUpdate();
		} else {
			int pid = getProcessPID(process);
			if (pid != HALT_PID) {
				if (process->toFork) {
					process->toFork = false;
					cloneProcess(PID);
				}
				if (!process->blockedOnSem) {
					if (process->waiting) {
						enqueueItem(&waitingQueue, process);
					} else {
						enqueueItemInPriority(&readyQueue, process, process->priority);
					}
				}
			}
		}
	}
	ProcessDescriptor *newProcess = dequeueHighestPriority(&readyQueue);
	if (newProcess == NULL) {
		newProcess = getProcess(HALT_PID);
	}

	int newPid = getProcessPID(newProcess);
	PID = newPid;

	return newProcess->stack; // Returned stack is aligned to 8 (not 16)
}

uint64_t countProcesses() {
	int count = 0;
	for (int i = 0; i < maxProcessCount; i++) {
		if (processes[i].state != PROCESS_DEAD) {
			count++;
		}
	}
	return count;
}

void setFocus(uint8_t tty) {
	changeFocusView(tty);
	/*for (int i = 0; i < sizeof(processes) / sizeof(*processes); i++) {*/
	/*if (processes[i].state != PROCESS_DEAD && processes[i].tty == tty) {*/
	/*focusPID = i;*/
	/*return;*/
	/*}*/
	/*}*/
	/*focusPID = -1;*/
}

struct ProcessDescriptor *getProcess(int pid) {
	return &processes[pid];
}
