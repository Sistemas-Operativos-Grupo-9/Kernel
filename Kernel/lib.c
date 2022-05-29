#include <stddef.h>
#include <stdint.h>

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

// https://www.techiedelight.com/implement-strncpy-function-c/
char *strncpy(char *destination, const char *source, uint64_t num) {
	// return if no memory is allocated to the destination
	if (destination == NULL) {
		return NULL;
	}

	// take a pointer pointing to the beginning of the destination string
	char *ptr = destination;

	// copy first `num` characters of C-string pointed by source
	// into the array pointed by destination
	while (*source && num--) {
		*destination = *source;
		destination++;
		source++;
	}

	// null terminate destination string
	*destination = '\0';

	// the destination is returned by standard `strncpy()`
	return ptr;
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

// https://stackoverflow.com/questions/32560167/strncmp-implementation
int strncmp(const char *s1, const char *s2, uint64_t n) {
	while (n && *s1 && (*s1 == *s2)) {
		++s1;
		++s2;
		--n;
	}
	if (n == 0) {
		return 0;
	} else {
		return (*(unsigned char *)s1 - *(unsigned char *)s2);
	}
}
