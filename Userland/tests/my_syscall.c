#include <stdint.h>
#include <syscall.h>
#include <shared-lib/print.h>

int64_t my_getpid() { return getpid(); }

int64_t my_create_process(int64_t (*func)(uint64_t argc, char *argv[]), uint64_t argc, char *argv[]) {
	int pid;
	switch (pid = fork()) {
	case 0:
		func(argc, argv);
		exit(0);

	case -1:
		return -1;

	default:
		return pid;
	}
}

int64_t my_nice(uint64_t pid, uint64_t newPrio) {
	return setprio(pid, newPrio);
}

int64_t my_kill(uint64_t pid) { return kill(pid); }

int64_t my_block(uint64_t pid) { return setblock(pid, true); }

int64_t my_unblock(uint64_t pid) { return setblock(pid, false); }

SID my_sem_open(char *sem_id) {
	return semOpen(sem_id);
}

SID my_sem_init(char *sem_id, uint64_t initialValue) {
	return semInit(sem_id, initialValue);
}

int64_t my_sem_wait(SID sem) { return semWait(sem); }

int64_t my_sem_post(SID sem) { return semPost(sem); }

int64_t my_sem_close(SID sem) { return semClose(sem); }

void my_yield() { yield(); }

int64_t my_wait(int64_t pid) { return waitpid(pid); }
