#include "interrupts.h"
#include "process.h"
#include "registers.h"
#include "video.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define GENERAL_PROTECTION_EXCEPTION_ID 13

extern struct RegistersState registersState;

void printReg(uint8_t viewNumber, uint64_t value, const char *name) {
	print(viewNumber, name);
	print(viewNumber, " = ");
	printHexPrefix(viewNumber);
	printUnsignedN(viewNumber, value, 16, 16);
}

void printRegs(uint8_t viewNumber, uint64_t value1, const char *name1,
               uint64_t value2, const char *name2) {
	printReg(viewNumber, value1, name1);
	print(viewNumber, "   -   ");
	printReg(viewNumber, value2, name2);
	printChar(viewNumber, '\n');
}

void printRegisters(uint8_t viewNumber) {
	struct RegistersState s = registersState;
	printRegs(viewNumber, s.rdi, "rdi", s.rsi, "  rsi");
	printRegs(viewNumber, s.rax, "rax", s.rbx, "  rbx");
	printRegs(viewNumber, s.rcx, "rcx", s.rdx, "  rdx");
	printRegs(viewNumber, s.rip, "rip", s.rsp, "  rsp");
	printRegs(viewNumber, s.rbp, "rbp", s.flags, "flags");
	printRegs(viewNumber, s.r8, " r8", s.r9, "   r9");
	printRegs(viewNumber, s.r10, "r10", s.r11, "  r11");
	printRegs(viewNumber, s.r12, "r12", s.r13, "  r13");
	printRegs(viewNumber, s.r14, "r14", s.r15, "  r15");
}

void exceptionDispatcher(int exception, uint64_t rip) {
	struct ProcessDescriptor *process = getCurrentProcess();

	print(process->tty, "Exception: ");
	printUnsigned(process->tty, exception, 10);
	printChar(process->tty, '\n');
	if (exception == 0) {
		print(process->tty, "Division by zero\n");
	} else if (exception == 6) {
		print(process->tty, "Invalid opcode\n");
	}
	print(process->tty, "Instruction Pointer: ");
	printHexPointer(process->tty, (void *)rip);
	printChar(process->tty, '\n');

	// printHexPointer((void *)returnAddress);
	// printChar('\n');
	_storeRegisters();
	printRegisters(process->tty);
	// print("Finished\n");
	// while (1);
	terminateProcess();
}