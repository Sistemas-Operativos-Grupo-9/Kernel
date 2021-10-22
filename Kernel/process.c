
#include "process.h"
#include "interrupts/interrupts.h"
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

void unpauseProcesses() {
	while (getLength(&waitingQueue)) {
		ProcessDescriptor *process = dequeueItem(&waitingQueue);
		process->waiting = false;
		enqueueItem(&readyQueue, process);
	}
}

void enqueueHalt() { enqueueItem(&readyQueue, &processes[HALT_PID]); }

extern void _nextProcess();
uint64_t readTTY(uint8_t tty, char *buf, uint64_t count) {
	uint64_t read;
	_sti();
	for (read = 0; read < count && buf[read] != '\n'; read++) {
		while (inputAvailable(tty) == 0) {
			// _nextProcess();
			getCurrentProcess()->waiting = true;
			__asm__("int $0x81");

			// _nextProcess();
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

void printClosingProcess(int retCode) {
	ProcessDescriptor *process = getCurrentProcess();
	// print(process->tty, "Process ");
	print(process->tty, process->name);
	print(process->tty, " -> ");
	printInt(process->tty, retCode, 10);
	printChar(process->tty, '\n');
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
	createProcess(process->tty, process->name, NULL, 0, true);

	process->active = false;
	_killAndNextProcess();
}

void terminateProcess() {
	ProcessDescriptor *process = getCurrentProcess();
	process->active = false;
	_killAndNextProcess();
}

void processReturned() {
	_cli();
	register int retCode __asm__("eax");
	printClosingProcess(retCode);
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
	*--stack = 0; // This value is changed in _switchContext
	*--stack = (uint64_t)0;
	*--stack = (uint64_t)0;

	struct ProcessDescriptor haltProcess = {
	    .tty = 0,
	    .name = "halt",
	    .fdTable =
	        {
	            (struct FileDescriptor){
	                .read = (int (*)(uint8_t, char *, uint64_t))readTTY},
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

int createProcess(uint8_t tty, char *name, char **argv, int argc,
                  bool restartOnFinish) {
	struct Module *module = getModule(name);
	if (module == NULL)
		return -1;
	START_LOCK;
	uint64_t pid = getFreePID();
	void *entryPoint = (void *)(pid * 0x100000 + 0x500000);
	uint64_t *stack =
	    (uint64_t
	         *)(entryPoint + 0x100000 -
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
	*--stack = 0; // This value is changed in _switchContext
	*--stack = (uint64_t)argc;
	*--stack = (uint64_t)argv;

	processes[pid] = (struct ProcessDescriptor){
	    .tty = tty,
	    .name = name,
	    .fdTable =
	        {
	            (struct FileDescriptor){
	                .read = (int (*)(uint8_t, char *, uint64_t))readTTY},
	            (struct FileDescriptor){
	                .write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
	            (struct FileDescriptor){
	                .write = (int (*)(uint8_t, char *, uint64_t))writeTTY},
	        },
	    .active = true,
	    .restart = restartOnFinish,
	    .stack = stack,
	    .entryPoint = entryPoint};
	enqueueItem(&readyQueue, &processes[pid]);
	END_LOCK;
	return pid;
}

int getProcessPID(ProcessDescriptor *process) { return process - processes; }

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