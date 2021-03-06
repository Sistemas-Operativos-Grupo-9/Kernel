/* _loader.c */
#include <string.h>

extern char bss;
extern char endOfBinary;
extern char got;
extern char gotEnd;

int main(int argc, char **argv);

int _start(int argc, char **argv) {
	// Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	for (uint64_t *i = (uint64_t *)&got; i < (uint64_t *)&gotEnd; i++) {
		*i += (uint64_t)(char *)&_start;
	}

	return main(argc, argv);
}
