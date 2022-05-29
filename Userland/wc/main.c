#include <shared-lib/print.h>
#include <string.h>
#include <syscall.h>

int main() {
	char buf[16];
	size_t c = 0;
	while (true) {
		int r = read(0, buf, sizeof(buf), 0);
		if (r == -1) {
			break;
		}
		c += tokencountn(buf, '\n', r);
	}
	printUnsigned(c, 10);
	putchar('\n');
	return 0;
}