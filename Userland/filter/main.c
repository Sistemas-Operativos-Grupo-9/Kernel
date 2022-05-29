#include "string.h"
#include <syscall.h>

int main() {
	char buf[16];
	while (true) {
		int r = read(0, buf, sizeof(buf), 0);
		if (r == -1) {
			break;
		}
		char filteredBuf[r];
		r = filterCharsN(buf, "aeiou", r, filteredBuf);
		write(1, filteredBuf, r);
	}
	return 0;
}