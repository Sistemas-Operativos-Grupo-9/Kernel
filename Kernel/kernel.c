#include "idtLoader.h"
#include "keyboard.h"
#include "myUtils.h"
#include "process.h"
#include "time.h"
#include <graphics/basicVideo.h>
#include <graphics/video.h>
#include <lib.h>
#include <moduleLoader.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t *mem_amount = (size_t *)(0x0000000000005A00 + 132);

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern uint8_t endOfKernelStack;
extern uint8_t startOfModules;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void *getStackBase() { return &endOfKernelStack; }

void *initializeKernelBinary() {
	clearBSS(&bss, &endOfKernel - &bss);
	initScreen();

	char buffer[10];

#define printHexPointer(ptr) printHexPointer(0, ptr);
#define printChar(str) putchar(0, str);
#define print(str) puts(0, str);

	print("[x64BareBones]\n");

	print("CPU Vendor:");
	print(cpuVendor(buffer));
	printChar('\n');

	print("[Loading modules]\n");

	if (loadModules(&startOfModules)) {
		print("FAILED TO LOAD MODULES!");
	} else {
		// struct Module *modules = (struct Module *)&endOfKernelStack;
		// uint32_t moduleCount =
		//     *(uint32_t *)((uint8_t *)modules + sizeof(struct Module) * 32);
		// for (int i = 0; i < moduleCount; i++) {
		// 	struct Module *module = &((struct Module *)(modules))[i];
		// 	print("Module: ");
		// 	print(module->name);
		// 	print(" loaded at address ");
		// 	printHexPointer(module->address);
		// 	printChar('\n');
		// }
	}
	print("[Done]\n\n");

	print("[Initializing kernel's binary]\n");
	// readBackup();

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

#undef print
#undef printChar
#undef printHexPointer

	return getStackBase();
}

int main() {
	load_idt();

	// setCursorAt(0, 0, 0);
	// clear(0);
	clear(0);
	clear(1);

#define PRINT_MODULE(moduleName)                                               \
	do {                                                                       \
		struct Module *module = getModule(moduleName);                         \
		if (module == NULL) {                                                  \
			puts(0, "ERROR!\n");                                               \
			break;                                                             \
		}                                                                      \
		puts(0, "Module \"");                                                  \
		puts(0, module->name);                                                 \
		puts(0, "\" loaded in address ");                                      \
		printHexPointer(0, module->address);                                   \
		putchar(0, '\n');                                                      \
	} while (0)

	// PRINT_MODULE("random");
	// PRINT_MODULE("shell");
	// PRINT_MODULE("mandelbrot");

#undef PRINT_MODULE

	// createProcess(1, "random", sampleCodeModuleAddress, (uint64_t *)shell -
	// 1);
	initializeHaltProcess();
	char *helpArgs[] = {"shell", "--print-help"};
	char *args[] = {"shell"};

	createProcess(0, "rerun", helpArgs, 2, true);
	/*createProcess(0, "rerun", args, 1, true);*/
	createProcess(1, "rerun", args, 1, true);
	createProcess(2, "rerun", args, 1, true);
	createProcess(3, "rerun", args, 1, true);
	createProcess(4, "rerun", args, 1, true);
	createProcess(5, "rerun", args, 1, true);

	_startScheduler();

	return 0;
}
