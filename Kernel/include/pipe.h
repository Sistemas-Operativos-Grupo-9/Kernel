#pragma once

#include "queue.h"
#include "semaphore.h"
#include <stdbool.h>
#include <stdint.h>
#define PIPE_SIZE 1024
#define MAX_PIPES 128

typedef struct Pipe {
	bool active;
	SID lock;
	char data[PIPE_SIZE];
	uint64_t nread;  // number of bytes read
	uint64_t nwrite; // number of bytes written
	int readopen;    // read fd is still open
	int writeopen;   // write fd is still open
} Pipe;

typedef int PIPID;

int pipeInit(int *pipe);
int pipeOpen(int fd);
int pipeClose(int fd);

void pipeIncRead(int fd);
void pipeIncWrite(int fd);
void pipeDecRead(int fd);
void pipeDecWrite(int fd);

int pipeRead(int fd, char *buf, int n, uint64_t timeout);
int pipeWrite(int fd, char *buf, int n);
Pipe *getPipe(PIPID pipid);
void pipePrintList();
