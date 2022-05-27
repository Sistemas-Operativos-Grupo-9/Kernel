#pragma once

#include "queue.h"
#include "semaphore.h"
#include <stdbool.h>
#define PIPE_SIZE 1024
#define MAX_PIPES 128

typedef struct Pipe {
	bool active;
	SID lock;
	char data[PIPE_SIZE];
	uint8_t nread;  // number of bytes read
	uint8_t nwrite; // number of bytes written
	int readopen;   // read fd is still open
	int writeopen;  // write fd is still open
} Pipe;

typedef int PIPID;

int pipeInit(int *pipe);
int pipeOpen(int fd);
int pipeClose(int fd);

int pipeDup2(int olderFd, int newFd);

int pipeRead(int fd, char *buf, int n);
int pipeWrite(int fd, char *buf, int n);
Pipe *getPipe(PIPID pipid);
void pipePrintList();