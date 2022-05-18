#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdlib.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
void *memmove(void *s1, const void *s2, uint64_t n);
int memcmp(const void *ptr1, const void *ptr2, uint64_t num);

char *cpuVendor(char *result);

uint64_t strlen(char *str);
void strcpy(char *dst, const char *src);
char *strncpy(char *destination, const char *source, uint64_t num);
int strcmp(char *str1, char *str2);
int strncmp(const char *s1, const char *s2, uint64_t n);

#endif
