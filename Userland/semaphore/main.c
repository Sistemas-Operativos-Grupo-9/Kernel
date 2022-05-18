#include "print.h"
#include "stdio.h"
#include "syscall.h"




void main() {
	puts("PID: ");
	printUnsigned(getpid(), 10);
	putchar('\n');
	SID sem = semInit("hola", 1);
	semWait(sem);
	semPrintList();
}
