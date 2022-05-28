#include "print.h"
#include "stdio.h"
#include "syscall.h"
#include <shared-lib/print.h>




void main() {
	SID sem = semInit("fork", 1);
	microsleep(100);

	int p;
	switch (p = fork()) {
		case 0:
			semWait(sem);
			puts("Child: ");
			printUnsigned(getpid(), 10);
			putchar('\n');
			semPost(sem);
			break;
		default:
			semWait(sem);
			puts("Parent: ");
			printUnsigned(getpid(), 10);
			puts(" | Child: ");
			printUnsigned(p, 10);
			putchar('\n');
			semPost(sem);
			waitpid(p);
			break;
	}
}
