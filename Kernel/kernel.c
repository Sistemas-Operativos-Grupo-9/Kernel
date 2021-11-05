#include "basicVideo.h"
#include "interrupts/idtLoader.h"
#include "interrupts/keyboard.h"
#include "interrupts/time.h"
#include "myUtils.h"
#include "process.h"
#include <lib.h>
#include <moduleLoader.h>
#include <null.h>
#include <stdbool.h>
#include <stdint.h>
#include <video.h>

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
#define printChar(str) printChar(0, str);
#define print(str) print(0, str);

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
			print(0, "ERROR!\n");                                              \
			break;                                                             \
		}                                                                      \
		print(0, "Module \"");                                                 \
		print(0, module->name);                                                \
		print(0, "\" loaded in address ");                                     \
		printHexPointer(0, module->address);                                   \
		printChar(0, '\n');                                                    \
	} while (0)

	// PRINT_MODULE("random");
	// PRINT_MODULE("shell");
	// PRINT_MODULE("mandelbrot");

#undef PRINT_MODULE

	// createProcess(1, "random", sampleCodeModuleAddress, (uint64_t *)shell -
	// 1);
	initializeHaltProcess();
	char *helpArgs[] = {"--print-help"};

	createProcess(0, "shell", helpArgs, 1, true);
	// createProcess(0, "game", NULL, 0, true);
	createProcess(1, "shell", NULL, 0, true);

	createProcess(2, "hangman", NULL, 0, true);
	createProcess(3, "chrono", NULL, 0, true);
	char *watchArgs[] = {"simple-date"};
	createProcess(4, "watch", watchArgs, 1, true);
	createProcess(5, "sudoku", NULL, 0, true);

	_startScheduler();

	return 0;
}
