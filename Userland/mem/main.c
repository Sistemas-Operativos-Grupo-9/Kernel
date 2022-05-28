
#include "syscall.h"
#include "shared-lib/print.h"


void main(int argc, char **argv) {
	
	char *buf = ourMalloc(100);
	char *buf2 = ourMalloc(100);
	puts("Buf: ");
	printHexPointer(buf);
	putchar('\n');
	puts("Buf2: ");
	printHexPointer(buf2);
	putchar('\n');
	ourFree(buf);
	ourFree(buf2);

}
