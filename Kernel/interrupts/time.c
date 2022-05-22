#include "process.h"
#include <time.h>

#define TICKS_PER_SECOND 25

static unsigned long ticks = 0;
extern bool schedulerEnabled;

void timer_handler() {
	ticks++;
	timerUpdate();
}

int ticks_elapsed() { return ticks; }

uint64_t microseconds_elapsed() {
	return ticks * 1000 * 1000 / TICKS_PER_SECOND;
}
int seconds_elapsed() { return ticks / TICKS_PER_SECOND; }
