#include "print.h"
#include "stdio.h"
#include "syscall.h"
#include <shared-lib/print.h>

void main() {
	int r, w;
	pipe(&r, &w);
	write(w, "hola", 5);
	close(w);
	puts("Written!\n");
	char *buf = ourMalloc(5);

	puts("Reading!\n");
	int bytes = read(r, buf, 5, 0);

	puts("Reading!\n");
	int bytes2 = read(r, buf, 5, 0);

	printInt(bytes, 10);
	putchar('\n');
	printInt(bytes2, 10);
	putchar('\n');

	puts(buf);
	/*pipePrintList();*/
	close(r);
	ourFree(buf);
}
