#include "interrupts.h"

bool getIF() {
	uint64_t flags;
	__asm__ __volatile__("pushf;"
	                     "pop %0"
	                     : "=rm"(flags)::"memory");
	return (flags & 1 << 9) != 0;
}