#include <print.h>
#include <stdio.h>
#include <time.h>

int main() {
	char str[] = "loop";

	for (int i = 0; i < 100; i++) {
		putchar(str[i % 4]);
		wait();
	}
	putchar('\n');

	return 0;
}