#include <stddef.h>

typedef struct memState {
	size_t totalMemory; // in bytes
	size_t heapStart;   // address
	size_t usedMemory;  // in bytes
	size_t fragmentsAmount;
} MemoryState;
