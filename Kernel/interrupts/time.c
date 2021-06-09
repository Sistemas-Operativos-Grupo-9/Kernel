#include <time.h>
#include "process.h"

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	// nextProcess();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
