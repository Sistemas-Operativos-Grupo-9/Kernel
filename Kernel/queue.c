#include "queue.h"
#include "interrupts.h"
#include "lock.h"
#include <stddef.h>

void enqueueItem(Queue *queue, void *item) {
	if ((queue->head + 1) % QUEUE_SIZE == queue->tail) {
		// TODO: error
	}
	LOCK(queue->items[queue->head++] = item;
	     if (queue->head >= QUEUE_SIZE) queue->head = 0;)
}

void *dequeueItem(Queue *queue) {
	if (queue->head == queue->tail) {
		return NULL;
	}
	void *ret;
	LOCK(ret = queue->items[queue->tail++];
	     if (queue->tail >= QUEUE_SIZE) queue->tail = 0;)
	return ret;
}

uint64_t getLength(Queue *queue) {
	int length = queue->head - queue->tail;
	if (length < 0) {
		length += QUEUE_SIZE;
	}
	return (uint64_t)length;
}

void queueIterate(Queue *queue, void (*fn)(void *)) {
	for (uint16_t i = queue->tail; i != queue->head; i = (i + 1) % QUEUE_SIZE) {
		fn(queue->items[i]);
	}
}
