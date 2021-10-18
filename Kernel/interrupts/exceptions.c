#include "process.h"
#include "video.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define GENERAL_PROTECTION_EXCEPTION_ID 13

extern void _printRegisters();

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

	_printRegisters();
	// print("Finished\n");
	// while (1);
	terminateProcess();
}