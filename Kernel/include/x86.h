#include <stdint.h>

uint8_t in(uint16_t port);

void out(uint16_t port, uint8_t value);

uint64_t xchg(volatile uint64_t *addr, uint64_t newval);
