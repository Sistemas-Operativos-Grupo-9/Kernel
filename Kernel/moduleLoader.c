#include <lib.h>
#include <moduleLoader.h>
#include <null.h>
#include <stdint.h>
#include <string.h>
#include <video.h>

extern uint8_t endOfKernel;
#define BACKUP_LOCATION &endOfKernel

static struct Module modules[32] = {};
static uint32_t moduleCount = 0;

void readBackup() {
	for (int i = 0; i < sizeof(modules) / sizeof(*modules); i++) {
		modules[i] = ((struct Module *)(BACKUP_LOCATION))[i];
	}
	moduleCount = *(uint32_t *)(BACKUP_LOCATION + sizeof(modules));
}

void addModule(struct Module *modules, uint32_t *moduleCount, char *name,
               void *address, uint64_t size) {
	modules[*moduleCount] = (struct Module){.address = address, .size = size};
	strcpy(modules[*moduleCount].name, name);
	(*moduleCount)++;
}
struct Module *getModule(char *name) {
	for (int i = 0; i < moduleCount; i++) {
		if (strcmp(modules[i].name, name) == 0) {
			return &modules[i];
		}
	}
	return NULL;
}

static void loadModule(struct Module *modules, uint32_t *moduleCount,
                       uint8_t **module, uint8_t **targetModuleAddress);
static uint32_t readUint32(uint8_t **address);
static void readName(uint8_t **address, char *buf);

void loadModules(void *payloadStart) {
	int i;
	uint8_t *currentModule = (uint8_t *)payloadStart;
	uint32_t totalModuleCount = readUint32(&currentModule);
	uint8_t *targetModuleAddress = (uint8_t *)BACKUP_LOCATION;
	struct Module *modulesBackup = (struct Module *)targetModuleAddress;
	uint32_t *moduleCountBackup =
	    (uint32_t *)(targetModuleAddress += sizeof(modules));
	targetModuleAddress += sizeof(totalModuleCount);

	for (i = 0; i < totalModuleCount; i++)
		loadModule(modulesBackup, moduleCountBackup, &currentModule,
		           &targetModuleAddress);

	// Backup data before losing it when cleaning bss
	// for (i = 0; i < sizeof(modules) / sizeof(*modules); i++) {
	// 	((struct Module *)(0x400000))[i] = modules[i];
	// }
	// *(uint32_t *)(0x400000 + sizeof(modules)) = totalModuleCount;
}

static void loadModule(struct Module *modules, uint32_t *moduleCount,
                       uint8_t **module, uint8_t **targetModuleAddress) {
	char name[32];
	readName(module, name);
	uint32_t moduleSize = readUint32(module);

	// print("  Will copy module at ");
	// printHexPointer(*module);
	// print(" to ");
	// printHexPointer(targetModuleAddress);
	// print(" (");
	// printUnsigned(moduleSize, 10);
	// print(" bytes)");

	memcpy(*targetModuleAddress, *module, moduleSize);
	addModule(modules, moduleCount, name, *targetModuleAddress, moduleSize);
	*module += moduleSize;
	*targetModuleAddress += moduleSize;

	// print(" [Done]\n");
}

static void readName(uint8_t **address, char *buf) {
	do {
		*buf++ = **address;
	} while (*((*address)++) != '\0');
}

static uint32_t readUint32(uint8_t **address) {
	uint32_t result = *(uint32_t *)(*address);
	*address += sizeof(uint32_t);
	return result;
}
