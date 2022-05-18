#pragma once

#include <stdint.h>

#define QUEUE_SIZE 128
typedef struct Queue {
	void *items[QUEUE_SIZE];
	uint16_t head;
	uint16_t tail;
} Queue;
void enqueueItem(Queue *queue, void *item);
void *dequeueItem(Queue *queue);
uint64_t getLength(Queue *queue);
void queueIterate(Queue *queue, void (*fn)(void *));
