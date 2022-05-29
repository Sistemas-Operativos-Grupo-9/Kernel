#include <graphics/video.h>
#include <lib.h>
#include <moduleLoader.h>
#include <stddef.h>
#include <stdint.h>

static struct Module modules[32] = {};
static uint32_t moduleCount = 0;

void addModule(char *name, void *address, uint64_t size) {
	modules[moduleCount++] =
	    (struct Module){.address = address, .size = size, .name = name};
}
struct Module *getModule(char *name) {
	for (int i = 0; i < moduleCount; i++) {
		if (strcmp(modules[i].name, name) == 0) {
			return &modules[i];
		}
	}
	return NULL;
}

static void loadModule(uint8_t **module);
static uint32_t readUint32(uint8_t **address);
static void readName(uint8_t **address);

int loadModules(void *payloadStart) {
	int i;
	uint8_t *currentModule = (uint8_t *)payloadStart;
	uint32_t totalModuleCount = readUint32(&currentModule);

	for (i = 0; i < totalModuleCount; i++)
		loadModule(&currentModule);

	return 0;
}

static void loadModule(uint8_t **module) {
	char *name = (char *)*module;
	readName(module);
	uint32_t moduleSize = readUint32(module);

	addModule(name, *module, moduleSize);
	*module += moduleSize;
}

static void readName(uint8_t **address) {
	while (*((*address)++) != '\0')
		;
}

static uint32_t readUint32(uint8_t **address) {
	uint32_t result = *(uint32_t *)(*address);
	*address += sizeof(uint32_t);
	return result;
}
