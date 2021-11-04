
#include "process.h"
#include "interrupts/interrupts.h"
#include "interrupts/time.h"
#include "lib.h"
#include "lock.h"
#include "moduleLoader.h"
#include "null.h"
#include "queue.h"
#include "video.h"
#include <views.h>
#define NO_PID -1
#define HALT_PID 0

static struct ProcessDescriptor processes[256];
static uint64_t maxProcessCount = sizeof(processes) / sizeof(*processes);
extern bool schedulerEnabled;
bool schedulerEnabled = false;
extern void *ret00;

Queue readyQueue;
Queue waitingQueue;

extern int PID;
int PID = NO_PID;

static int focusPID = 0;

static int getFreePID() {
	static int lastPID = -1;
	lastPID++;

	if (lastPID >= sizeof(processes) / sizeof(*processes)) {
		lastPID = 0;
	}
	while (processes[lastPID].active)
		lastPID++;

	return lastPID;
}

struct ProcessDescriptor *getCurrentProcess() {
	if (PID == NO_PID)
		return NULL;
	return &processes[PID];
}

struct ProcessDescriptor *getFocusedProcess() {
	if (focusPID == NO_PID)
		return NULL;
	return &processes[focusPID];
}

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

void enqueueHalt() { enqueueItem(&readyQueue, &processes[HALT_PID]); }

extern void _nextProcess();
uint64_t readTTY(uint8_t tty, char *buf, uint64_t count, uint64_t timeout) {
	uint64_t start = microseconds_elapsed() / 1000;
	uint64_t read;
	_sti();
	for (read = 0; read < count && buf[read] != '\n'; read++) {
		while (
		    inputAvailable(tty) == 0 &&
		    (timeout == 0 || microseconds_elapsed() / 1000 < start + timeout)) {
			// _nextProcess();
			waitForIO();

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
uint64_t writeTTY(uint8_t tty, char *buf, uint64_t count) {
	for (int i = 0; i < count; i++)
		writeOutput(tty, buf[i]);
	return count;
}

void waitForIO() {
	getCurrentProcess()->waiting = true;
	_yield();
}

void printClosingProcess(int retCode) {
	ProcessDescriptor *process = getCurrentProcess();
}

bool killProcess(int pid) {
	if (!processes[pid].active)
		return false;
	processes[pid].toKill = true;
	return true;
}

void restartProcess() {
	ProcessDescriptor *process = getCurrentProcess();
	print(process->tty, "Restarting process...");
	createProcess(process->tty, process->name, process->argv, process->argc,
	              true);

	process->active = false;
	_killAndNextProcess();
	__asm__("iretq");
}

void terminateProcess() {
	ProcessDescriptor *process = getCurrentProcess();
	process->active = false;
	_killAndNextProcess();
	__asm__("iretq");
}

void processReturned() {
	_cli();
	register int retCode __asm__("eax");
	printClosingProcess(retCode);
	childDeadUpdate();
	if (processes[PID].restart)
		restartProcess();
	else
		terminateProcess();
}

void haltMain() {
	while (true) {
		__asm__("hlt");
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
	// *--stack = 0; // This value is changed in _switchContext
	uint64_t *leaveStack = stack;
	*--stack = (uint64_t)leaveStack;
	*--stack = (uint64_t)0;
	*--stack = (uint64_t)0;
	stack -= 14; // 16 - 2

	struct ProcessDescriptor haltProcess = {
	    .tty = 0,
	    .name = "halt",
	    .fdTable =
	        {
	            (struct FileDescriptor){
	                .read =
	                    (int (*)(uint8_t, char *, uint64_t, uint64_t))readTTY},
	            (struct FileDescriptor){
	                .write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
	            (struct FileDescriptor){
	                .write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
	        },
	    .active = true,
	    .restart = false,
	    .stack = stack,
	    .entryPoint = haltMain};
	processes[HALT_PID] = haltProcess;
}
extern uint8_t endOfModules;

#define PROCESSES_START &endOfModules
#define PROCESS_MEMORY 0x200000

int createProcess(uint8_t tty, char *name, char **argv, int argc,
                  bool restartOnFinish) {
	struct Module *module = getModule(name);
	if (module == NULL)
		return -1;
	START_LOCK;
	uint64_t pid = getFreePID();
	void *entryPoint = (void *)(pid * PROCESS_MEMORY + PROCESSES_START);
	uint64_t *stack =
	    (uint64_t
	         *)(entryPoint + PROCESS_MEMORY -
	            8); // Position process stack at the end of it's memory region

	memcpy((void *)entryPoint, (void *)module->address, module->size);

	uint64_t *stackInit = stack;
	*stack = (uint64_t)processReturned;
	*--stack = (uint64_t)entryPoint;
	*--stack = 0;
	*--stack = (uint64_t)stackInit;
	*--stack = 0x202;
	*--stack = 8;
	*--stack = (uint64_t)entryPoint;
	// *--stack = 0; // This value is changed in _switchContext
	uint64_t *leaveStack = stack;
	*--stack = (uint64_t)leaveStack;
	*--stack = (uint64_t)argv;
	*--stack = (uint64_t)argc;
	stack -= 14; // 16 - 2

	processes[pid] = (struct ProcessDescriptor){
	    .tty = tty,
	    .name = name,
	    .fdTable =
	        {
	            (struct FileDescriptor){
	                .read =
	                    (int (*)(uint8_t, char *, uint64_t, uint64_t))readTTY},
	            (struct FileDescriptor){
	                .write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
	            (struct FileDescriptor){
	                .write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
	        },
	    .active = true,
	    .restart = restartOnFinish,
	    .stack = stack,
	    .entryPoint = entryPoint,
	    .argc = argc,
	    .argv = argv,
	};
	enqueueItem(&readyQueue, &processes[pid]);
	END_LOCK;
	return pid;
}

int getProcessPID(ProcessDescriptor *process) { return process - processes; }

void *doSwitch(bool pushCurrent, uint64_t *stackPointer) {
	if (pushCurrent) {
		ProcessDescriptor *process = getCurrentProcess();
		process->initialized = true;
		process->stack = stackPointer;
		if (process->toKill) {
			processReturned();
		}
		int pid = getProcessPID(process);
		if (pid != HALT_PID) {
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

	return newProcess->stack;
}

uint64_t countProcesses() {
	int count = 0;
	for (int i = 0; i < maxProcessCount; i++) {
		if (processes[i].active) {
			count++;
		}
	}
	return count;
}

void setFocus(uint8_t tty) {
	changeFocusView(tty);
	for (int i = 0; i < sizeof(processes) / sizeof(*processes); i++) {
		if (processes[i].active && processes[i].tty == tty) {
			focusPID = i;
			return;
		}
	}
	focusPID = -1;
}

struct ProcessDescriptor *getProcess(int pid) {
	return &processes[pid];
}