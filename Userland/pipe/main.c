#include "print.h"
#include "stdio.h"
#include "syscall.h"
#include <shared-lib/print.h>

void main() {
	int r, w;
	pipe(&r, &w);
	pipePrintList();
	close(r);
	close(w);
}
