#include <print.h>
#include <stdio.h>
#include <syscall.h>

int main() {
	puts("There are ");
	printUnsigned(proccount(), 10);
	puts(" processes running.\n");
	return 0;
}