#include <print.h>
#include <stdio.h>
#include <syscall.h>
#include <time.h>

int main() {
	Time time = gettime();
	printUnsignedN(time.hours, 2, 10);
	puts("::");
	printUnsignedN(time.minutes, 2, 10);
	puts("::");
	printUnsignedN(time.seconds, 2, 10);
	putchar('\n');

	return 0;
}