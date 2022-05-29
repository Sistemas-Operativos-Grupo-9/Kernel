#include <stdbool.h>
#include <stdio.h>
#include <syscall.h>

enum Commands { RESUME_PAUSE, STOP, QUIT, NOTHING };

enum Commands parse() {
	KeyStroke key = readKeyStroke(10);
	if (key.isTimeout)
		return NOTHING;
	if (key.isPrintable) {
		if (key.data == 'q') {
			return QUIT;
		} else if (key.data == ' ') {
			return RESUME_PAUSE;
		}
	} else if (key.data == '\b') {
		return STOP;
	}
	return NOTHING;
}

void printTime(uint64_t millis) {
	int minutes = millis / (1000 * 60);
	int seconds = millis / (1000) % 60;
	int milliseconds = millis % 1000;

	printUnsignedN(minutes, 2, 10);
	puts("\' ");
	printUnsignedN(seconds, 2, 10);
	puts("\" ");
	printUnsignedN(milliseconds, 3, 10);
}

void loop() {

	bool running = false;
	bool updated = false;
	uint64_t start = 0;
	uint64_t acum = 0;

	printTime(0);
	while (true) {
		updated = running;
		switch (parse()) {
		case RESUME_PAUSE:
			running = !running;
			if (running) {
				start = millis();
			} else {
				acum += millis() - start;
			}
			updated = true;
			break;
		case STOP:
			acum = 0;
			running = false;
			updated = true;
			break;
		case QUIT:
			return;
			break;
		case NOTHING:
			break;
		}
		if (updated) {
			putchar('\r');
			printTime(acum + (running ? millis() - start : 0));
		}
	}
}

int main() {

	loop();
	putchar('\n');

	return 0;
}