
#define ZERO_EXCEPTION_ID 0
#include "video.h"

void exceptionDispatcher(int exception) {
	print("Exception: ");
	printUnsigned(exception, 10);
	printChar('\n');
	while (1);
}