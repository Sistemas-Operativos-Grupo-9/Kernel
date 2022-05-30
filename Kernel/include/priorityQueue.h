#pragma once
#include "queue.h"
#include <stddef.h>

#define PRIORITY_COUNT 3

// queues[0] is highest priority
typedef struct PriorityQueue {
	Queue queues[PRIORITY_COUNT];
} PriorityQueue;

void *dequeueHighestPriority(PriorityQueue *priorityQueue);
void enqueueItemInPriority(PriorityQueue *priorityQueue, void *item,
                           int priority);
