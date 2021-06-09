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
	initVideo();
	initColors();
	clear(0);
	print(0, infoBuffer);

	return getStackBase();
}

void run(void *address) {
	uint64_t returnCode = ((EntryPoint)address)();
	// print(0, "\n\nReturn code: ");
	// printUnsigned(returnCode, 16);
	// printChar(0, '\n');
}

int main()
{
	load_idt();

	// while(true);
	setCursorAt(0, 0, 0);
	clear(0);
	// print(0, "Hola");


	createProcess(0);
	run(sampleCodeModuleAddress);
	// run(shell);
	createProcess(1);
	setFocus(1);
	nextProcess();
	clear(1);
	run(shell);

	// print("[Kernel Main]");
	// printChar('\n');
	// print("  Sample code module at ");
	// printHexPointer(sampleCodeModuleAddress);
	// printChar('\n');
	// print("  Calling the sample code module returned: ");
	// printChar('\n');
	// printChar('\n');

	// print("  Sample data module at ");
	// printHexPointer(sampleDataModuleAddress);
	// printChar('\n');
	// print("  Sample data module contents: ");
	// print((char*)sampleDataModuleAddress);
	// printChar('\n');

	print(0, "\n[Finished]\n");
	while (1);
	// setCursorAt(0, 0);
	// printTestData();
	
	return 0;
}
