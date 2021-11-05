/* _loader.c */
#include <string.h>

extern char bss;
extern char endOfBinary;

int main(char **argv, int argc);

int _start(char **argv, int argc) {
	// Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	return main(argv, argc);
}
