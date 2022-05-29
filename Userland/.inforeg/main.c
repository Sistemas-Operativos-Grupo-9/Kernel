#include "registers.h"
#include <stdio.h>
#include <string.h>
#include <syscall.h>

void printReg(uint64_t value, const char *name) {
	puts(name);
	puts(" = ");
	printHexPrefix();
	printUnsignedN(value, 16, 16);
}

void printRegs(uint64_t value1, const char *name1, uint64_t value2,
               const char *name2) {
	printReg(value1, name1);
	puts("   -   ");
	printReg(value2, name2);
	putchar('\n');
}

struct RegistersState zeroed = {0};

int main() {
	struct RegistersState s = getRegisters();
	if (memcmp(&s, &zeroed, sizeof(struct RegistersState)) == 0) {
		puts("Registers state is zeroed. \n"
		     "Save a snapshot of the registers state by pressing F12 and try "
		     "again.\n");
		return -1;
	}

	printRegs(s.rdi, "rdi", s.rsi, "  rsi");
	printRegs(s.rax, "rax", s.rbx, "  rbx");
	printRegs(s.rcx, "rcx", s.rdx, "  rdx");
	printRegs(s.rip, "rip", s.rsp, "  rsp");
	printRegs(s.rbp, "rbp", s.flags, "flags");
	printRegs(s.r8, " r8", s.r9, "   r9");
	printRegs(s.r10, "r10", s.r11, "  r11");
	printRegs(s.r12, "r12", s.r13, "  r13");
	printRegs(s.r14, "r14", s.r15, "  r15");

	return 0;
}