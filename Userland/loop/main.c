#include <print.h>
#include <stdio.h>
#include <syscall.h>

int main() {
	char str[] = "loop";

	for (int i = 0; i < 100; i++) {
		putchar(str[i % 4]);
		nanosleep(1000 * 1000 / 10);
	}
	putchar('\n');

	return 0;
}