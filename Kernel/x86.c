#include <x86.h>

uint8_t in(uint16_t port) {
	uint8_t value;

	__asm__ __volatile__("inb %w1, %0;" : "=a"(value) : "Nd"(port));

	return value;
}

void out(uint16_t port, uint8_t value) {
	__asm__ __volatile__("outb %b1, %w0;" : : "Nd"(port), "a"(value));
}

// https://github.com/mit-pdos/xv6-public/blob/eeb7b415dbcb12cc362d0783e41c3d1f44066b17/x86.h
inline uint64_t xchg(volatile uint64_t *addr, uint64_t newval) {
	uint64_t result;

	// The + in "+m" denotes a read-modify-write operand.
	__asm__ __volatile__("lock; xchgl %0, %1"
	                     : "+m"(*addr), "=a"(result)
	                     : "1"(newval)
	                     : "cc");
	return result;
}