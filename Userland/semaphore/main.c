#include "print.h"
#include "stdio.h"
#include "syscall.h"




void main() {
	SID sem = semInit("hola", 1);
	while (true) {
		semWait(sem);
		puts("hola\n");
	}
}
