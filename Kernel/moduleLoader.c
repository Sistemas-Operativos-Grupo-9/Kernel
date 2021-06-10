#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <video.h>
#include <string.h>
#include <null.h>

static struct Module modules[32] = {};
static uint32_t moduleCount = 0;

void readBackup() {
	for (int i = 0; i < sizeof(modules) / sizeof(*modules); i++) {
		modules[i] = ((struct Module *)(0x400000))[i];
	}
	moduleCount = *(uint32_t *)(0x400000 + sizeof(modules));
}

void addModule(char *name, void *address, uint64_t size) {
	modules[moduleCount] = (struct Module) {.address = address, .size = size};
	strcpy(modules[moduleCount].name, name);
	moduleCount++;
}
struct Module *getModule(char *name) {
	for (int i = 0; i < moduleCount; i++) {
		if (strcmp(modules[i].name, name) == 0) {
			return &modules[i];
		}
	}
	return NULL;
}

static void loadModule(uint8_t ** module, uint8_t ** targetModuleAddress);
static uint32_t readUint32(uint8_t ** address);
static void readName(uint8_t ** address, char *buf);

void loadModules(void * payloadStart)
{
	memset(modules, 0, sizeof(modules));
	moduleCount = 0;
	int i;
	uint8_t * currentModule = (uint8_t*)payloadStart;
	uint32_t totalModuleCount = readUint32(&currentModule);
	uint8_t *targetModuleAddress = (uint8_t*)0x400000;
	targetModuleAddress += sizeof(modules);
	targetModuleAddress += sizeof(totalModuleCount);

	for (i = 0; i < totalModuleCount; i++)
		loadModule(&currentModule, &targetModuleAddress);


	// Backup data before losing it when cleaning bss
	for (i = 0; i < sizeof(modules) / sizeof(*modules); i++) {
		((struct Module *)(0x400000))[i] = modules[i];
	}
	*(uint32_t *)(0x400000 + sizeof(modules)) = totalModuleCount;
}

static void loadModule(uint8_t ** module, uint8_t ** targetModuleAddress)
{
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
	addModule(name, *targetModuleAddress, moduleSize);
	*module += moduleSize;
	*targetModuleAddress += moduleSize;

	// print(" [Done]\n");
}

static void readName(uint8_t ** address, char *buf) {
	do {
		*buf++ = **address;
	} while (*((*address)++) != '\0');
}

static uint32_t readUint32(uint8_t ** address)
{
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
