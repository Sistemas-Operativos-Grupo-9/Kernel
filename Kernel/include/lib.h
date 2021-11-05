#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
void *memmove(void *s1, const void *s2, uint64_t n);

char *cpuVendor(char *result);

uint64_t strlen(char *str);
void strcpy(char *dst, const char *src);
int strcmp(char *str1, char *str2);

#endif