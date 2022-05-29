#include "graphics/video.h"
#include "interrupts.h"
#include "process.h"
#include "registers.h"
#include "shared-lib/print.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define GENERAL_PROTECTION_EXCEPTION_ID 13

extern struct RegistersState registersState;

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

void printRegisters() {
	struct RegistersState s = registersState;
	printRegs(s.rdi, "rdi", s.rsi, "  rsi");
	printRegs(s.rax, "rax", s.rbx, "  rbx");
	printRegs(s.rcx, "rcx", s.rdx, "  rdx");
	printRegs(s.rip, "rip", s.rsp, "  rsp");
	printRegs(s.rbp, "rbp", s.flags, "flags");
	printRegs(s.r8, " r8", s.r9, "   r9");
	printRegs(s.r10, "r10", s.r11, "  r11");
	printRegs(s.r12, "r12", s.r13, "  r13");
	printRegs(s.r14, "r14", s.r15, "  r15");
}

void exceptionDispatcher(int exception, struct RegistersState *registers) {
	puts("Exception: ");
	printUnsigned(exception, 10);
	putchar('\n');
	if (exception == 0) {
		puts("Division by zero\n");
	} else if (exception == 6) {
		puts("Invalid opcode\n");
	}

	// printHexPointer((void *)returnAddress);
	// putchar('\n');
	_storeRegisters(registers);
	printRegisters();
	// puts("Finished\n");
	// while (1);
	terminateProcess();
}
