
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

__attribute__((used, optimize("-fno-tree-loop-distribute-patterns"))) void *
memset(void *destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char *dst = (char *)destination;

	while (length--)
		dst[length] = chr;

	return destination;
}

__attribute__((used, optimize("-fno-tree-loop-distribute-patterns"))) void *
memcpy(void *restrict destination, const void *restrict source,
       uint64_t length) {
	uint8_t *d = (uint8_t *)destination;
	const uint8_t *s = (const uint8_t *)source;
	for (uint64_t i = 0; i < length; i++)
		d[i] = s[i];

	return destination;
}

__attribute__((used, optimize("-fno-tree-loop-distribute-patterns"))) void *
memmove(void *s1, const void *s2, uint64_t n) {
	char *dest = (char *)s1;
	const char *src = (const char *)s2;

	if (dest <= src) {
		while (n--) {
			*dest++ = *src++;
		}
	} else {
		src += n;
		dest += n;

		while (n--) {
			*--dest = *--src;
		}
	}

	return s1;
}

__attribute__((used, optimize("-fno-tree-loop-distribute-patterns"))) int
memcmp(const void *ptr1, const void *ptr2, uint64_t num) {
	unsigned char u1, u2;

	for (; num--; ptr1++, ptr2++) {
		u1 = *(unsigned char *)ptr1;
		u2 = *(unsigned char *)ptr2;
		if (u1 != u2) {
			return (u1 - u2);
		}
	}
	return 0;
}