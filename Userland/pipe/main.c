#include "print.h"
#include "stdio.h"
#include "syscall.h"
#include <shared-lib/print.h>

void main() {
	int a, c;
	pipeInit(&a);
	pipeInit(&c);
	pipePrintList();
}
