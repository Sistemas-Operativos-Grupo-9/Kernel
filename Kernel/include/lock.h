#include "interrupts.h"

extern int lock_count;

#define START_LOCK                                                             \
	do {                                                                       \
		lock_count++;                                                          \
		_cli();                                                                \
	} while (0)
#define END_LOCK                                                               \
	do {                                                                       \
		if (--lock_count == 0)                                                 \
			_sti();                                                            \
	} while (0)

#define LOCK(code)                                                             \
	START_LOCK;                                                                \
	code END_LOCK;
