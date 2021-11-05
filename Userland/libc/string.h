#pragma once

#include "stdint.h"

uint64_t strlen(char *str);
void strcpy(char *dst, const char *src);
int strcmp(char *str1, char *str2);

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
void *memmove(void *s1, const void *s2, uint64_t n);
int memcmp(const void *ptr1, const void *ptr2, uint64_t num);