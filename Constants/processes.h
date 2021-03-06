#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum ProcessState {
	PROCESS_DEAD = 0,
	PROCESS_ACTIVE,
	PROCESS_ZOMBIE
} ProcessState;

struct Process {
	int pid;
	char name[20];
	void *entryPoint;
	void *stackStart;
	bool waiting;
	bool blocked;
	uint8_t priority;
	ProcessState state;
};
