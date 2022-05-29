#include "pipe.h"
#include "process.h"
#include "semaphore.h"
#include "shared-lib/print.h"
#include "syscall.h"
#include "time.h"

// https://github.com/mit-pdos/xv6-public/blob/master/pipe.c

static Pipe pipes[MAX_PIPES];

static PIPID getFreePIPID() {
	static int lastSID = 0;

	while (pipes[lastSID].active) {
		lastSID++;
		if (lastSID >= sizeof(pipes) / sizeof(*pipes)) {
			lastSID = 0;
		}
	}

	return lastSID;
}

int pipeInit(int *pipe) {
	PIPID pipid = getFreePIPID();
	Pipe *p = getPipe(pipid);

	p->active = true;
	p->lock = semInit("", 1); // Need a better name?
	p->nread = 0;
	p->nwrite = 0;
	p->readopen = 1;
	p->writeopen = 1;

	*pipe = pipid;

	return 0;
}

int pipeOpen(int fd) {
	Pipe *p = getPipe(fd);

	if (p->active) {
		return 0;
	} else {
		return -1;
	}
}
int pipeClose(int fd) {
	Pipe *p = getPipe(fd);

	if (p->active) {
		p->active = false;
		semClose(p->lock);
		return 0;
	} else {
		return -1;
	}
}

static void tryClose(int fd) {
	Pipe *pipe = getPipe(fd);
	semWait(pipe->lock);
	if (pipe->readopen == 0 && pipe->writeopen == 0) {
		pipeClose(fd);
	} else {
		semPost(pipe->lock);
	}
}

void pipeIncRead(int fd) {
	Pipe *pipe = getPipe(fd);
	semWait(pipe->lock);
	pipe->readopen++;
	semPost(pipe->lock);
}

void pipeIncWrite(int fd) {
	Pipe *pipe = getPipe(fd);
	semWait(pipe->lock);
	pipe->writeopen++;
	semPost(pipe->lock);
}

void pipeDecRead(int fd) {
	Pipe *pipe = getPipe(fd);
	semWait(pipe->lock);
	pipe->readopen--;
	semPost(pipe->lock);
	tryClose(fd);
}

void pipeDecWrite(int fd) {
	Pipe *pipe = getPipe(fd);
	semWait(pipe->lock);
	pipe->writeopen--;
	semPost(pipe->lock);
	tryClose(fd);
}

int pipeRead(PIPID fd, char *buf, int n, uint64_t timeout) {
	Pipe *pipe = getPipe(fd);
	if (!pipe->active) {
		return -2;
	}

	// mut_wait(getSemaphore(pipe->lock), &pipe->lock);
	semWait(pipe->lock);
	// Si no hay nada que leer, esperar
	uint64_t start = microseconds_elapsed() / 1000;
	while (pipe->nread == pipe->nwrite &&
	       (timeout == 0 || microseconds_elapsed() / 1000 < start + timeout) &&
	       pipe->writeopen > 0) {
		semPost(pipe->lock);
		_yield();
		semWait(pipe->lock);
	}

	int nread = 0;
	while (nread < n && pipe->nread != pipe->nwrite) {
		buf[nread++] = pipe->data[pipe->nread++ % PIPE_SIZE];
	}

	if (nread == 0 && pipe->writeopen == 0) {
		semPost(pipe->lock);
		return -1;
	}

	// mut_signal(getSemaphore(pipe->lock), &pipe->lock);
	semPost(pipe->lock);
	return nread;
}

int pipeWrite(int fd, char *buf, int n) {
	Pipe *pipe = &pipes[fd];
	if (!pipe->active) {
		return -1;
	}

	// mut_wait(getSemaphore(pipe->lock), &pipe->lock);
	semWait(pipe->lock);

	int nwrite = 0;
	while (nwrite < n && pipe->nwrite < PIPE_SIZE) {
		pipe->data[pipe->nwrite++ % PIPE_SIZE] = buf[nwrite++];
	}

	// mut_signal(getSemaphore(pipe->lock), &pipe->lock);
	semPost(pipe->lock);

	return nwrite;
}

Pipe *getPipe(PIPID pipid) { return &pipes[pipid]; }

void pipePrint(PIPID pipid) {
	Pipe *p = getPipe(pipid);
	if (p->active) {
		puts("Pipe: ");
		printInt(pipid, 10);
		putchar('\n');
		puts("Read end open count:");
		printUnsigned(p->readopen, 10);
		putchar('\n');
		puts("Write end open count:");
		printUnsigned(p->writeopen, 10);
		putchar('\n');
		printBlockedProcesses(p->lock);
	}
}

void pipePrintList() {
	for (int i = 0; i < MAX_PIPES; i++) {
		Pipe *pipe = getPipe(i);
		if (pipe->active) {
			pipePrint(i);
		}
	}
}
