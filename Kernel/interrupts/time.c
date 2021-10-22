#include "process.h"
#include <time.h>

static unsigned long ticks = 0;
extern bool schedulerEnabled;

void timer_handler() { ticks++; }

int ticks_elapsed() { return ticks; }

uint64_t nanoseconds_elapsed() { return ticks * 1000 * 1000 / 18; }
int seconds_elapsed() { return ticks / 18; }

void wait(uint64_t tickDuration) {
	uint64_t end = ticks + tickDuration;
	while (ticks < end)
		;
}