#include "interrupts.h"
#include <stdatomic.h>

extern _Atomic(int) lock_count;

void startLock();
void endLock();

#define LOCK(code)                                                             \
	startLock();                                                               \
	code endLock();
