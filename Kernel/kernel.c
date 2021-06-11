#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <video.h>
#include "basicVideo.h"
#include <stdbool.h>
#include "interrupts/time.h"
#include "interrupts/keyboard.h"
#include "interrupts/idtLoader.h"
#include <null.h>
#include "myUtils.h"
#include "process.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{

	char buffer[10];
	char infoBuffer[256];
	int infoIndex = 0;
	#define printChar(ch) infoBuffer[infoIndex++] = ch
	#define print(str) {char *ptr = str;while(*ptr) printChar(*(ptr++));}
	#define printHexPointer(ptr) print("0x"); unsignedToString((uint64_t)ptr, 16, infoBuffer + infoIndex, 16); infoIndex += 16

	print("[x64BareBones]\n");

	print("CPU Vendor:");
	print(cpuVendor(buffer));
	printChar('\n');

	print("[Loading modules]\n");

	loadModules(&endOfKernelBinary);
	print("[Done]\n\n");

	print("[Initializing kernel's binary]\n");
	clearBSS(&bss, &endOfKernel - &bss);
	readBackup();

	print("  text: ");
	printHexPointer(&text);
	printChar('\n');
	print("  rodata: ");
	printHexPointer(&rodata);
	printChar('\n');
	print("  data: ");
	printHexPointer(&data);
	printChar('\n');
	print("  bss: ");
	printHexPointer(&bss);
	printChar('\n');

	print("[Done]\n");

	printChar('\n');
	printChar('\0');

	#undef print
	#undef printChar
	#undef printHexPointer
	initScreen();
	clear(0);
	print(0, infoBuffer);

	return getStackBase();
}

int main()
{
	load_idt();

	// setCursorAt(0, 0, 0);
	// clear(0);
	clear(0);
	clear(1);

	#define PRINT_MODULE(moduleName) 							\
		do {													\
			struct Module *module = getModule(moduleName);  	\
			if (module == NULL) {print(0, "ERROR!\n"); break;}	\
			print(0, "Module \"");								\
			print(0, module->name);								\
			print(0, "\" loaded in address ");					\
			printHexPointer(0, module->address);				\
			printChar(0, '\n');									\
		} while(0)

	// PRINT_MODULE("random");
	// PRINT_MODULE("shell");

	#undef PRINT_MODULE

	// createProcess(1, "random", sampleCodeModuleAddress, (uint64_t *)shell - 1);
	char *helpArgs[] = {"--print-help"};

	createProcess(0, "shell", helpArgs, 1, true);
	createProcess(1, "shell", NULL, 0, true);

	setFocus(0);
	
	_startScheduler();
	
	return 0;
}
