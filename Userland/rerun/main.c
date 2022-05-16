#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <syscall.h>

int main(int argc, char **argv) {
	while (true) {
		char *module = argv[0];
		char **args = NULL;
		if (argc > 1) {
			args = &argv[1];
		}
		execve(module, args, argc - 1);
	}
	return 0;
}
