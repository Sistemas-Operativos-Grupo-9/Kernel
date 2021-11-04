
#include "string.h"

uint64_t strlen(char *str) {
	char *end = str;
	while (*end++ != '\0')
		;
	return end - str - 1;
}

void strcpy(char *dst, const char *src) {
	do {
		*dst++ = *src++;
	} while (*(dst - 1) != '\0');
}

int strcmp(char *str1, char *str2) {
	do {
		if (*str1 > *str2)
			return 1;
		else if (*str1 < *str2)
			return -1;
	} while (*str1++ != '\0' && *str2++ != '\0');
	return 0;
}