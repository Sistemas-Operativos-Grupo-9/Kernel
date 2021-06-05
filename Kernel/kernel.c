#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <video.h>
#include <stdbool.h>
#include "interrupts/time.h"
#include "interrupts/keyboard.h"
#include "interrupts/idtLoader.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

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
	clearBSS(&bss, &endOfKernel - &bss);
	initVideo();

	print("[x64BareBones]\n");

	print("CPU Vendor:");
	print(cpuVendor(buffer));
	printChar('\n');

	print("[Initializing kernel's binary]\n");


	print("  text: 0x");
	printHexPointer(&text);
	printChar('\n');
	print("  rodata: 0x");
	printHexPointer(&rodata);
	printChar('\n');
	print("  data: 0x");
	printHexPointer(&data);
	printChar('\n');
	print("  bss: 0x");
	printHexPointer(&bss);
	printChar('\n');

	print("[Done]\n");

	print("[Loading modules]\n");
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	print("[Done]\n\n");

	printChar('\n');

	return getStackBase();
}

int main()
{
	load_idt();

	print("[Kernel Main]");
	printChar('\n');
	print("  Sample code module at 0x");
	printHexPointer(sampleCodeModuleAddress);
	printChar('\n');
	print("  Calling the sample code module returned: ");
	printHexPointer((void *)(uint64_t)((EntryPoint)sampleCodeModuleAddress)());
	printChar('\n');
	printChar('\n');

	print("  Sample data module at 0x");
	printHexPointer(sampleDataModuleAddress);
	printChar('\n');
	print("  Sample data module contents: ");
	print((char*)sampleDataModuleAddress);
	printChar('\n');

	print("[Finished]\n");

	setCursorAt(0, 0);
	clear();
	printTestData();
	
	while (true) {
		
	}
	return 0;
}
