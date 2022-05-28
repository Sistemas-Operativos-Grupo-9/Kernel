#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <syscall.h>

int main(int argc, char **argv) {
	while (true) {
		char *module = argv[0];
		char **args = argc > 0 ? &argv[1] : NULL;
		int p;
		switch(p = fork()) {
			case 0:
				exec(module, args);
			default:
				waitpid(p);
		}
	}
	return 0;
}
