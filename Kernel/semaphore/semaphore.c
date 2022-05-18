#include "semaphore.h"
#include "graphics/video.h"
#include "lib.h"
#include "process.h"
#include "queue.h"

typedef struct Semaphore {
	bool active;
	char name[10];
	semValue value;
	Queue blockedProcesses;
} Semaphore;

static Semaphore semaphores[MAX_SEMAPHORES];

static SID getFreeSID() {
	static int lastSID = 0;

	while (semaphores[lastSID].active) {
		lastSID++;
		if (lastSID >= sizeof(semaphores) / sizeof(*semaphores)) {
			lastSID = 0;
		}
	}

	return lastSID;
}

Semaphore *getSemaphore(SID sid) { return &semaphores[sid]; }

SID semInit(const char *name, semValue value) {
	SID sid = getFreeSID();

	Semaphore *sem = getSemaphore(sid);

	sem->active = true;
	sem->value = value;

	strncpy(sem->name, name, sizeof(sem->name));

	return sid;
}

bool semClose(SID sid) {
	Semaphore *sem = getSemaphore(sid);
	if (!sem->active) {
		return true;
	}

	sem->active = false;
	return false;
}

bool semWait(SID sid) {
	Semaphore *sem = getSemaphore(sid);
	if (!sem->active)
		return true;
	//
	
	enqueueItem(&sem->blockedProcesses, getCurrentProcess());
	return false;
}

bool semPost(SID sid) {
	Semaphore *sem = getSemaphore(sid);
	if (!sem->active)
		return true;

	// Wake up blocked process
	return false;
}

static SID semFindByName(const char *name) {
	for (int i = 0; i < MAX_SEMAPHORES; i++) {

		Semaphore *sem = getSemaphore(i);
		if (sem->active) {
			if (strncmp(name, sem->name, sizeof(sem->name)) == 0) {
				return i;
			}
		}
	}
	return -1;
}

SID semOpen(const char *name) { return semFindByName(name); }

static void printProcess(ProcessDescriptor *process) {
	uint8_t tty = getCurrentProcess()->tty;

	printInt(tty, getProcessPID(process), 10);
	putchar(tty, ' ');
}

void semPrint(SID sid) {
	uint8_t tty = getCurrentProcess()->tty;

	Semaphore *sem = getSemaphore(sid);
	if (sem->active) {
		puts(tty, "Semaphore #:\n");
		puts(tty, "    value: ");
		printUnsigned(tty, sem->value, 10);
		putchar(tty, '\n');
		if (sem->name[0] == '\0') {
			puts(tty, "    no name\n");
		} else {
			puts(tty, "    name: ");
			puts(tty, sem->name);
			putchar(tty, '\n');
		}
		puts(tty, "    blocked processes: ");
		queueIterate(&sem->blockedProcesses, (void (*)(void *))printProcess);
		putchar(tty, '\n');

	} else {
		puts(tty, "Semaphore #: inactive\n");
	}
}

void semPrintList() {
	for (int i = 0; i < MAX_SEMAPHORES; i++) {
		Semaphore *sem = getSemaphore(i);
		if (sem->active) {
			semPrint(i);
		}
	}
}
