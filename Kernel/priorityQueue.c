#include "priorityQueue.h"

void *dequeueHighestPriority(PriorityQueue *priorityQueue) {
	for (int i = 0; i < PRIORITY_COUNT; i++) {
		void *best = dequeueItem(&priorityQueue->queues[i]);
		if (best != NULL) {
			return best;
		}
	}
	return NULL;
}

void enqueueItemInPriority(PriorityQueue *priorityQueue, void *item,
                           int priority) {
	enqueueItem(&priorityQueue->queues[priority], item);
}
