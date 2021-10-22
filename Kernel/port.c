#include <port.h>

uint8_t in(uint16_t port) {
	uint8_t value;

	__asm__("inb %w1, %0;" : "=a"(value) : "Nd"(port));

	return value;
}

void out(uint16_t port, uint8_t value) {
	__asm__("outb %b1, %w0;" : : "Nd"(port), "a"(value));
}