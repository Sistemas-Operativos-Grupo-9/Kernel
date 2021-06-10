#include "queue.h"


void enqueueItem(Queue *queue, void *item) {
    if ((queue->head + 1) % QUEUE_SIZE == queue->tail) {
        // TODO: error
    }
    queue->items[queue->head++] = item;
    if (queue->head >= QUEUE_SIZE)
        queue->head = 0;
}

void *dequeueItem(Queue *queue) {
    if (queue->head == queue->tail) {
        // TODO: error
    }
    void *ret = queue->items[queue->tail++];
    if (queue->tail >= QUEUE_SIZE)
        queue->tail = 0;
    return ret;
}