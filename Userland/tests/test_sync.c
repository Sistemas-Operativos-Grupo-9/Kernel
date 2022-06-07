#include "my_syscall.h"
#include "test_util.h"
#include <shared-lib/print.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "tests.h"

#define SEM_NAME "sem"
#define TOTAL_PAIR_PROCESSES 3

int64_t *global = (int64_t *)0x400200; // shared memory (kernel.ld: &testsMemory)

void slowInc(int64_t *p, int64_t inc) {
	int64_t aux = *p;
	my_yield(); // This makes the race condition highly probable
	aux += inc;
	*p = aux;
}

int64_t my_process_inc(uint64_t argc, char *argv[]) {
	uint64_t n;
	int8_t inc;
	int8_t use_sem;

	if (argc != 3)
		return -1;

	if ((n = satoi(argv[0])) <= 0)
		return -1;
	if ((inc = satoi(argv[1])) == 0)
		return -1;
	if ((use_sem = satoi(argv[2])) < 0)
		return -1;

	SID sem;
	if (use_sem)
		if ((sem = my_sem_open(SEM_NAME)) < 0) {
			puts("test_sync: ERROR opening semaphore\n");
			return -1;
		}

	uint64_t i;
	for (i = 0; i < n; i++) {
		if (use_sem)
			my_sem_wait(sem);
		slowInc(global, inc);
		if (use_sem)
			my_sem_post(sem);
	}

	return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
	uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

	if (argc != 2)
		return -1;

	SID sem = my_sem_init(SEM_NAME, 1);
	char *argvDec[] = {argv[0], "-1", argv[1], NULL};
	char *argvInc[] = {argv[0], "1", argv[1], NULL};

	*global = 0;

	uint64_t i;
	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		pids[i] = my_create_process(my_process_inc, 3, argvDec);
		pids[i + TOTAL_PAIR_PROCESSES] =
		    my_create_process(my_process_inc, 3, argvInc);
	}

	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		my_wait(pids[i]);
		my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
	}

	my_sem_close(sem);

	puts("Final value: ");
	printInt(*global, 10);
	putchar('\n');

	return 0;
}
