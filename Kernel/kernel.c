#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <video.h>
#include "basicVideo.h"
#include <stdbool.h>
#include "interrupts/time.h"
#include "interrupts/keyboard.h"
#include "interrupts/idtLoader.h"
#include "myUtils.h"
#include "process.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const shell = (void*)0x500000;
static void * const shell2 = (void*)0x600000;

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
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		shell
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	print("[Done]\n\n");

	print("[Initializing kernel's binary]\n");
	clearBSS(&bss, &endOfKernel - &bss);

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

void run(void *address) {
	// uint64_t returnCode = ((EntryPoint)address)();
	// print(0, "\n\nReturn code: ");
	// printUnsigned(returnCode, 16);
	// printChar(0, '\n');
}

int main()
{
	load_idt();

	setCursorAt(0, 0, 0);
	clear(0);
	clear(1);
	clear(2);
	// clear();

	// createProcess(1, "random", sampleCodeModuleAddress, (uint64_t *)shell - 1);

	memcpy(shell2, shell, shell2 - shell);
	createProcess(1, "shell", shell, (uint64_t *)(shell + 0x100000) - 1, true);
	createProcess(2, "shell2", shell2, (uint64_t *)(shell2 + 0x100000) - 1, false);

	setFocus(2);
	
	_startScheduler();


	print(0, "\n[Finished]\n");
	while (1);
	
	return 0;
}
