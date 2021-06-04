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

	print("[x64BareBones]\n");

	print("CPU Vendor:");
	print(cpuVendor(buffer));
	printChar('\n');

	print("[Loading modules]\n");
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	print("[Done]\n\n");

	print("[Initializing kernel's binary]\n");

	clearBSS(&bss, &endOfKernel - &bss);

	print("  text: 0x");
	printUnsigned((uint64_t)&text, 8, 16);
	printChar('\n');
	print("  rodata: 0x");
	printUnsigned((uint64_t)&rodata, 8, 16);
	printChar('\n');
	print("  data: 0x");
	printUnsigned((uint64_t)&data, 8, 16);
	printChar('\n');
	print("  bss: 0x");
	printUnsigned((uint64_t)&bss, 8, 16);
	printChar('\n');

	print("[Done]");
	printChar('\n');
	printChar('\n');
	return getStackBase();
}

int main()
{
	load_idt();
	print("[Kernel Main]");
	printChar('\n');
	print("  Sample code module at 0x");
	printUnsigned((uint64_t)sampleCodeModuleAddress, 8, 16);
	printChar('\n');
	print("  Calling the sample code module returned: ");
	printUnsigned(((EntryPoint)sampleCodeModuleAddress)(), 8, 16);
	printChar('\n');
	printChar('\n');

	print("  Sample data module at 0x");
	printUnsigned((uint64_t)sampleDataModuleAddress, 8, 16);
	printChar('\n');
	print("  Sample data module contents: ");
	print((char*)sampleDataModuleAddress);
	printChar('\n');

	print("[Finished]\n");

	int line = getCursorY();
	while (true) {
		// setCursorAt(0, line);
		// printChar(getLastChar());
		// printChar('\n');
		// printUnsigned(ticks_elapsed(), 10, 10);
	}
	return 0;
}
