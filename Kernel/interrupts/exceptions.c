#include "video.h"
#include "process.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define GENERAL_PROTECTION_EXCEPTION_ID 13

extern void _printRegisters();

void exceptionDispatcher(int exception) {
	struct ProcessDescriptor *process = getCurrentProcess();

	print(process->tty, "Exception: ");
	printUnsigned(process->tty, exception, 10);
	printChar(process->tty, '\n');

	
	// printHexPointer((void *)returnAddress);
	// printChar('\n');

	_printRegisters();
	// print("Finished\n");
	// while (1);
	restartProcess();
}