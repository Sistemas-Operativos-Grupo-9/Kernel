#include "video.h"


#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define GENERAL_PROTECTION_EXCEPTION_ID 13

void exceptionDispatcher(int exception) {
	print(0, "Exception: ");
	printUnsigned(exception, 10);
	printChar(0, '\n');
	
	// printHexPointer((void *)returnAddress);
	// printChar('\n');

	// _printRegisters();
	// print("Finished\n");
	while (1);
}