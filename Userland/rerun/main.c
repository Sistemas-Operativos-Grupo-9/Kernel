#include <shared-lib/print.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <syscall.h>

int main(int argc, char **argv) {
	while (true) {
		char *module = argv[0];
		char **args = argc > 0 ? &argv[1] : NULL;
		int p;
		switch (p = fork()) {
		case 0:
			if (!exec(module, args)) {
				puts("Module does not exist.");
				return -1;
			}
		default: {
			int res;
			if ((res = waitpid(p)) != 0)
				return res;
		}
		}
	}
	return 0;
}
