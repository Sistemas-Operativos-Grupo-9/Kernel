#include "process.h"
#include "graphics/video.h"
#include "interrupts.h"
#include "lib.h"
#include "lock.h"
#include "moduleLoader.h"
#include "queue.h"
#include "time.h"
#include <graphics/views.h>
#include <stddef.h>
#include <stdint.h>
#define NO_PID -1
#define HALT_PID 0

#define MAX_PROCESS_COUNT 256

static struct ProcessDescriptor processes[MAX_PROCESS_COUNT];
static uint64_t maxProcessCount = sizeof(processes) / sizeof(*processes);
extern bool schedulerEnabled;
bool schedulerEnabled = false;
extern void *ret00;

Queue readyQueue;
Queue waitingQueue;

extern int PID;
int PID = 0;

static int getFreePID() {
	static int lastPID = 0;

	while (processes[lastPID].state != PROCESS_DEAD) {
		lastPID++;
		if (lastPID >= sizeof(processes) / sizeof(*processes)) {
			lastPID = 0;
		}
	}

	return lastPID;
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
	while (getLength(&waitingQueue)) {
		ProcessDescriptor *process = dequeueItem(&waitingQueue);
		process->waiting = false;
		enqueueItem(&readyQueue, process);
	}
}

void timerUpdate() { unpauseProcesses(); }

void keypressUpdate() { unpauseProcesses(); }

void childDeadUpdate() { unpauseProcesses(); }

void semaphoreUpdate() { unpauseProcesses(); }

void enqueueHalt() { enqueueItem(&readyQueue, &processes[HALT_PID]); }

extern void _nextProcess();
int64_t readTTY(uint8_t tty, char *buf, uint64_t count, uint64_t timeout) {
	uint64_t start = microseconds_elapsed() / 1000;
	uint64_t read;
	if (hasEof(tty)) {
		setEof(tty, false);
		return -1;
	}
	_sti();
	for (read = 0; read < count && buf[read] != '\n'; read++) {
		while (
		    inputAvailable(tty) == 0 &&
		    (timeout == 0 || microseconds_elapsed() / 1000 < start + timeout)) {
			// _nextProcess();
			waitForIO();
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

void waitForIO() {
	getCurrentProcess()->waiting = true;
	_yield();
}

bool killProcess(int pid) {
	if (processes[pid].state == PROCESS_DEAD)
		return false;
	processes[pid].toKill = true;
	return true;
}

int waitPID(int pid) {
	ProcessDescriptor *process = getProcess(pid);
	while (process->state != PROCESS_ZOMBIE) {
		waitForIO();
	}
	int ret = process->returnCode;
	process->state = PROCESS_DEAD;
	return ret;
}

void initializeLogProcess() {
	processes[0].fdTable[1] = (struct FileDescriptor){
	    .write = (int (*)(ID, const char *, uint64_t))writeTTY,
	    .id = 0,
	};
}

void terminateProcess() {
	ProcessDescriptor *process = getCurrentProcess();
	process->state = PROCESS_ZOMBIE;
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
		// __asm__("nop");
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

	initializeProcessStack(&stack, entryPoint, args);

	processes[pid] = (struct ProcessDescriptor){
	    .name = module->name,
	    .entryPoint = entryPoint,
	    .fdTable =
	        {
	            (struct FileDescriptor){
	                .read =
	                    (int (*)(ID, const char *, uint64_t, uint64_t))readTTY,
	                .id = tty,
	            },
	            (struct FileDescriptor){
	                .write = (int (*)(ID, const char *, uint64_t))writeTTY,
	                .id = tty,
	            },
	            (struct FileDescriptor){
	                .write = (int (*)(ID, const char *, uint64_t))writeTTY,
	                .id = tty,
	            },
	        },
	    .state = PROCESS_ACTIVE,
	    .stack = stack,
	};
	enqueueItem(&readyQueue, &processes[pid]);
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

	initializeProcessStack(&stack, entryPoint, args);

	ProcessDescriptor *process = getProcess(pid);
	process->name = module->name;
	process->state = PROCESS_ACTIVE;
	process->stack = stack;
	enqueueItem(&readyQueue, &processes[pid]);

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
	getProcessBoundaries(pid, &start, &end);
	int newPid = getFreePID();
	getProcessBoundaries(newPid, &dstStart, &dstEnd);
	memcpy(dstStart, start, (uint64_t)end - (uint64_t)start + 1 * 8);
	ProcessDescriptor *process = getProcess(pid);
	ProcessDescriptor *newProcess = getProcess(newPid);
	*newProcess = *process;
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

	enqueueItem(&readyQueue, newProcess);
	return newPid;
}

void *doSwitch(bool pushCurrent, uint64_t *stackPointer) {
	if (pushCurrent) {
		ProcessDescriptor *process = getCurrentProcess();
		process->initialized = true;
		process->stack = stackPointer;
		if (process->toKill) {
			process->toKill = false;
			processReturned();
		}
		int pid = getProcessPID(process);
		if (pid != HALT_PID) {
			if (process->toFork) {
				process->toFork = false;
				cloneProcess(PID);
			}
			Queue *queue;
			if (process->waiting) {
				queue = &waitingQueue;
			} else {
				queue = &readyQueue;
			}
			enqueueItem(queue, process);
		}
	}
	if (getLength(&readyQueue) == 0) {
		enqueueHalt();
	}
	ProcessDescriptor *newProcess = dequeueItem(&readyQueue);

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
