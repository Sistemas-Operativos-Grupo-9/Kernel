#include "lock.h"

_Atomic(int) lock_count = 0;

void startLock() {
	lock_count++;
	_cli();
}

void endLock() {
	if (atomic_fetch_sub(&lock_count, 1) == 1)
		_sti();
}
