
#include "stdint.h"

void wait() {
    for (uint64_t i = 0; i < 40000000UL; i++) __asm__("nop");
}