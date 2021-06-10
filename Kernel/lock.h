#include "interrupts/interrupts.h"

#define LOCK(code)          \
    if (getIF()) {          \
        _cli();             \
        do {code} while(0); \
        _sti();             \
    } else {                \
        code                \
    }
