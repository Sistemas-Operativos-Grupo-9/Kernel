#include "processes.h"
#include "shared-lib/print.h"
#include "stdio.h"
#include "string.h"
#include "syscall.h"
#define MAX_PHILOSOPHERS 20
#define SEM_ANON ""

/* Consulted from:
 * - https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/
 * - Operating System Concepts, 10th Edition. Page 296
 */

#define LEFT (phnum + table->activePhilosophers - 1) % table->activePhilosophers
#define RIGHT (phnum + 1) % table->activePhilosophers

extern int sharedMemory;

typedef enum { EATING = 'E', THINKING = '.', HUNGRY = 'H' } PhiloState;

typedef enum { QUIT, ADD, REMOVE, DO_NOTHING } Command;

typedef struct PhilosopherStruct {
	SID sem;
	PhiloState state;
	int pid;
} Philosopher;

SID tableLock;

struct TableState {
	bool running;
	SID mutex;
	SID printLock;
	size_t activePhilosophers;
	Philosopher philosophers[MAX_PHILOSOPHERS];
} *table = (struct TableState *)0x0000000000400000; // &sharedMemory

// Function Signatures
void philosopher(int num);
void printTableState();
void incrementActivePhilosophers();
void decrementActivePhilosophers();
size_t getActivePhilosophers();

// Philosofers

void philoSetState(int phnum, PhiloState state) {

	semWait(table->printLock);
	table->philosophers[phnum].state = state;

	printTableState();
	semPost(table->printLock);
}

PhiloState philoGetState(int phnum) { return table->philosophers[phnum].state; }

int philoGetPID(int phnum) { return table->philosophers[phnum].pid; }

void philoSetPID(int phnum, int pid) { table->philosophers[phnum].pid = pid; }

SID philoGetSem(int phnum) { return table->philosophers[phnum].sem; }

void initPhilosofer(int phnum) {
	table->philosophers[phnum].state = THINKING;
	table->philosophers[phnum].sem = semInit(SEM_ANON, 0);
	table->philosophers[phnum].pid = -1; // value for idle philosofers
}

void addPhilosofer() {
	if (getActivePhilosophers() == MAX_PHILOSOPHERS) {
		return;
	}
	int nextPhilosopher = getActivePhilosophers();
	incrementActivePhilosophers();
	int pid;
	switch ((pid = fork())) {
	case 0:
		philosopher(nextPhilosopher);
		break;
	default:
		philoSetPID(nextPhilosopher, pid);
		break;
	}
}

int removePhilosofer() {
	int activePhilosophers = getActivePhilosophers();
	if (activePhilosophers == 1)
		return -1;

	decrementActivePhilosophers();

	int pid = philoGetPID(activePhilosophers);
	philoSetPID(activePhilosophers, -1);
	return pid;
}

/////////////////
// Table

void incrementActivePhilosophers() {
	semWait(tableLock);
	table->activePhilosophers++;
	semPost(tableLock);
}

void decrementActivePhilosophers() {
	semWait(tableLock);
	table->activePhilosophers--;
	semPost(tableLock);
}

size_t getActivePhilosophers() { return table->activePhilosophers; }

void test(int phnum) {
	if (philoGetState(phnum) == HUNGRY && philoGetState(LEFT) != EATING &&
	    philoGetState(RIGHT) != EATING) {

		philoSetState(phnum, EATING);
		semPost(philoGetSem(phnum));
	}
}

void pickupChopsticks(int phnum) {

	semWait(table->mutex);

	philoSetState(phnum, HUNGRY);

	test(phnum);

	semPost(table->mutex);

	if (philoGetState(phnum) != EATING) {
		semWait(philoGetSem(phnum));
	}
}

void putdownChopsticks(int phnum) {

	semWait(table->mutex);

	philoSetState(phnum, THINKING);

	test(LEFT);
	test(RIGHT);

	semPost(table->mutex);
}

void thinkWait(int phnum) { sleep(3); }

void eatWait(int phnum) { sleep(2); }

void philosopher(int num) {
	while (getActivePhilosophers() > num && table->running) {
		thinkWait(num);

		pickupChopsticks(num);

		eatWait(num);

		putdownChopsticks(num);
	}
	exit(0);
}

void printTableState() {
	if (!table->running) {
		return;
	}
	size_t currentPhilo = 0;
	while (currentPhilo < getActivePhilosophers()) {
		putchar(philoGetState(currentPhilo++));
	}
	putchar('\n');
}

Command parseInput() {
	KeyStroke key = readKeyStroke(1);

	if (key.isEOF)
		return QUIT;
	if (!key.isPrintable)
		return DO_NOTHING;

	switch (key.data) {
	case 'a':
	case 'A':
		return ADD;
	case 'r':
	case 'R':
		return REMOVE;
	case 'q':
	case 'Q':
		return QUIT;
	}
	return DO_NOTHING;
}

bool checkIfOtherIsRunning() {
	struct Process processes[256];
	size_t count = getProcesses(processes);
	size_t phyloCount = 0;
	for (size_t i = 0; i < count; i++) {
		if (strcmp("phylo", processes[i].name) == 0) {
			phyloCount++;
			if (phyloCount == 2)
				return true;
		}
	}
	return false;
}

void main(int argc, char **argv) {
	if (checkIfOtherIsRunning()) {
		puts("phylo is already running\n");
		return;
	}

	int i;
	int startingPhilosofers = 5;

	// initialize the semaphores
	table->mutex = semInit(SEM_ANON, 1);
	table->printLock = semInit(SEM_ANON, 1);
	tableLock = semInit(SEM_ANON, 1);

	table->activePhilosophers = 0;
	table->running = true;

	for (i = 0; i < MAX_PHILOSOPHERS; i++) {
		initPhilosofer(i);
	}

	for (i = 0; i < startingPhilosofers; i++) {
		addPhilosofer();
	}

	char command = DO_NOTHING;
	do {
		if (command == ADD) {
			addPhilosofer();
		}
		if (command == REMOVE) {
			int pid = removePhilosofer();
			waitpid(pid);
		}

	} while ((command = parseInput()) != QUIT);
	table->running = false;

	int pid2;
	while ((pid2 = removePhilosofer()) != -1) {
		waitpid(pid2);
	}
}