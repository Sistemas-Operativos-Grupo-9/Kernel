#include "print.h"
#include "stdio.h"
#include "syscall.h"
#include <shared-lib/print.h>



void main() {
	SID sem = semOpen("hola");
	printInt(sem, 10);

	semPost(sem);
}
