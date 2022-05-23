#include "semaphore.h"
#include "graphics/video.h"
#include "lib.h"
#include "process.h"
#include "queue.h"
#include <stdatomic.h>

typedef struct Semaphore {
	bool active;
	char name[10];
	semValue value;
	atomic_flag lock;
	Queue blockedProcesses;
} Semaphore;

static Semaphore semaphores[MAX_SEMAPHORES];

static mut_signal(atomic_flag *lock) {
	atomic_flag_clear(lock);
}

static mut_wait(Semaphore *sem, atomic_flag *lock) {
	while (atomic_flag_test_and_set(lock)) {
		ProcessDescriptor *process = getCurrentProcess();
		enqueueItem(&sem->blockedProcesses, process);
		process->waiting = true;
		_yield();
	}
}

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

	while (true) {
		mut_wait(sem, &sem->lock);

		if (sem->value > 0) {
			sem->value--;
			mut_signal(&sem->lock);
			break;
		}

		mut_signal(&sem->lock);
	}
	/*puts(getCurrentProcess()->tty, "s\n");*/
	
	return false;
}

bool semPost(SID sid) {
	Semaphore *sem = getSemaphore(sid);
	if (!sem->active)
		return true;

	if (sem->value++ == 0) {
		/*puts(getCurrentProcess()->tty, "p\n");*/
		// Wake up blocked process
		while (getLength(&sem->blockedProcesses) > 0) {
			dequeueItem(&sem->blockedProcesses);
		}

		semaphoreUpdate();
	}


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
