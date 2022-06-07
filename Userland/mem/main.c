
#include "shared-lib/print.h"
#include "syscall.h"

int main(int argc, char **argv) {
	MemoryState state = getMemoryState();
	puts("Total Memory: ");
	printUnsigned(state.totalMemory, 10);
	putchar('\n');
	puts("Used Memory: ");
	printUnsigned(state.usedMemory, 10);
	putchar('\n');
	puts("Fragments Amount: ");
	printUnsigned(state.fragmentsAmount, 10);
	putchar('\n');
	puts("Heap Start: ");
	printHexPointer((void *)state.heapStart);
	putchar('\n');

	return 0;
}
