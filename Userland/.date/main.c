#include <stdio.h>
#include <syscall.h>
#include <time.h>

int main() {
	char out[21];

	toISO8601(gettime(), out);
	puts("Current time: ");
	puts(out);
	putchar('\n');

	return 0;
}