#ifndef MODULELOADER_H
#define MODULELOADER_H

struct Module {
	char name[32];
	void *address;
	uint64_t size;
};

struct Module *getModule(char *name);
int loadModules(void *payloadStart);
void readBackup();

#endif