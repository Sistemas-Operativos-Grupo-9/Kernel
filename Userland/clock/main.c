#include <stdio.h>
#include <syscall.h>
#include <time.h>

int main() {
	char out[21];

	for (int i = 0; i < 10; i++) {
		putchar('\r');
		toISO8601(gettime(), out);
		puts("Current time: ");
		puts(out);

		millisleep(1000);
	}
	putchar('\n');

	return 0;
}