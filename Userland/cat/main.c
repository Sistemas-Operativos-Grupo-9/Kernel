#include <syscall.h>

int main() {
	char buf[16];
	while (true) {
		int r = read(0, buf, sizeof(buf), 0);
		if (r == -1) {
			break;
		}
		write(1, buf, r);
	}
	return 0;
}