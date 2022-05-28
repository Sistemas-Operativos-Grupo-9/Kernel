#include "idtLoader.h"
#include "keyboard.h"
#include "shared-lib/myUtils.h"
#include "process.h"
#include "time.h"
#include <graphics/basicVideo.h>
#include <graphics/video.h>
#include <shared-lib/print.h>
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
	initializeLogProcess();

	char buffer[10];

	puts("[x64BareBones]\n");

	puts("CPU Vendor:");
	puts(cpuVendor(buffer));
	putchar('\n');

	puts("[Loading modules]\n");

	if (loadModules(&startOfModules)) {
		puts("FAILED TO LOAD MODULES!");
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
	puts("[Done]\n\n");

	puts("[Initializing kernel's binary]\n");
	// readBackup();

	puts("  text: ");
	printHexPointer(&text);
	putchar('\n');
	puts("  rodata: ");
	printHexPointer(&rodata);
	putchar('\n');
	puts("  data: ");
	printHexPointer(&data);
	putchar('\n');
	puts("  bss: ");
	printHexPointer(&bss);
	putchar('\n');

	puts("[Done]\n");

	putchar('\n');

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

	initializeHaltProcess();
	char *helpArgs[] = {"shell", "--print-help", NULL};
	char *args[] = {"shell", NULL};

	/*createProcess(0, "fork", NULL, 0, true);*/
	createProcess(0, "rerun", helpArgs);
	createProcess(1, "rerun", args);
	createProcess(2, "rerun", args);
	createProcess(3, "rerun", args);
	createProcess(4, "rerun", args);
	createProcess(5, "rerun", args);

	_startScheduler();

	return 0;
}
