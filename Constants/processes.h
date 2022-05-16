#pragma once

#include <stdbool.h>

struct Process {
	int pid;
	char name[20];
	void *entryPoint;
	void *stackStart;
	bool waiting;
};
