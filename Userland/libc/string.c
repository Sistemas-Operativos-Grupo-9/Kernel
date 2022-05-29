
#include "string.h"
#include "memory_manager.h"

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

void strncpy(char *dst, const char *src, size_t n) {
	// https://stackoverflow.com/questions/14159625/implementation-of-strncpy
	size_t i = 0;
	while (i++ != n && (*dst++ = *src++))
		;
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

int strncmp(const char *s1, const char *s2, size_t n) {
	// from https://stackoverflow.com/questions/32560167/strncmp-implementation
	while (n && *s1 && (*s1 == *s2)) {
		++s1;
		++s2;
		--n;
	}
	if (n == 0)
		return 0;
	else
		return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

size_t tokencount(char *str, char c) {
	size_t count = 0;
	size_t i = 0;
	while (str[i] != '\0') {
		if (str[i] == c)
			count++;
		i++;
	}
	return i;
}

size_t deleteNCharsAtIndex(char *str, size_t index, size_t n) {
	size_t moved = 0;
	for (; moved < n && str[index + moved] != '\0'; moved++)
		;
	while (str[index + moved] != '\0') {
		str[index] = str[index + moved];
		index++;
	}
	str[index] = '\0';
	return moved;
}

void deleteCharAtIndex(char *str, size_t index) {
	deleteNCharsAtIndex(str, index, 1);
}

size_t filterTokens_getTokensAmount(char **tokens) {
	size_t count = 0;
	while (tokens[count] != NULL)
		count++;
	return count;
}

size_t filterTokens(char *str, char **tokens, char *dest) {
	size_t tokensAmount = filterTokens_getTokensAmount(tokens);
	size_t tokenSizes[tokensAmount];
	for (size_t i = 0; i < tokensAmount; i++)
		tokenSizes[i] = strlen(tokens[i]);

	size_t sLen = strlen(str);
	size_t strIndex = 0;
	size_t destIndex = 0;
	while (str[strIndex] != '\0') {
		for (size_t t = 0; t < tokensAmount; t++) {
			if (strIndex + tokenSizes[t] <= sLen &&
			    strncmp(str + strIndex, tokens[t], tokenSizes[t]) == 0) {
				strIndex += tokenSizes[t];
				// reset t to check for tokens on the new strIndex position
				t = -1;
			}
		}
		dest[destIndex++] = str[strIndex++];
	}
	dest[destIndex] = '\0';
	return destIndex - 1;
}

size_t filterChars(char *str, char *tokens, char *dest) {
	size_t tokensAmount = strlen(tokens);
	size_t strIndex = 0;
	size_t destIndex = 0;
	while (str[strIndex] != '\0') {
		for (size_t t = 0; t < tokensAmount; t++) {
			if (str[strIndex] == tokens[t]) {
				strIndex++;
				// reset t to check for tokens on the new strIndex position
				t = -1;
			}
		}
		dest[destIndex++] = str[strIndex++];
	}
	dest[destIndex] = '\0';
	return destIndex - 1;
}

char **split(const char *str, char delim, size_t *size) {
	size_t delimAmount = tokencount(str, delim);
	size_t lastDelimIndex = -1;

	// edge case: first char is delim -> ignore it
	if (str[0] == delim) {
		delimAmount--;
		lastDelimIndex = 0;
	}

	char **splitArray = ourMalloc((delimAmount + 1) * sizeof(char *));
	// error checking
	if (splitArray == NULL) {
		return NULL;
	}

	size_t splitIndex = 0;
	size_t strIndex = 1;

	while (str[strIndex - 1] != '\0') {
		if (str[strIndex] == delim || str[strIndex] == '\0') {
			if (splitIndex == delimAmount)
				break;

			size_t splitSize =
			    strIndex - lastDelimIndex; // includes the final '\0'

			// ignore the splits with no size
			if (splitSize <= 1) {
				lastDelimIndex = strIndex;
				strIndex++;
				continue;
			}
			splitArray[splitIndex] = ourMalloc(splitSize);
			// error checking
			if (splitArray[splitIndex] == NULL) {
				for (size_t i = 0; i < splitIndex; i++)
					ourFree(splitArray[splitIndex]);
				return NULL;
			}

			strncpy(splitArray[splitIndex], str + lastDelimIndex + 1,
			        splitSize - 1);
			splitArray[splitIndex][splitSize - 1] = '\0';

			splitIndex++;
			lastDelimIndex = strIndex;
		}
		strIndex++;
	}
	*size = splitIndex;
	return splitArray;
}

size_t split2(char *str, char delim, char **splitArray) {

	size_t splitIndex = 0;
	size_t strIndex = 0;

	while (str[strIndex] != '\0') {

		while (str[strIndex] == delim)
			str[strIndex++] = '\0';

		if (str[strIndex] == '\0')
			break;

		if (strIndex == 0 || str[strIndex - 1] == '\0')
			splitArray[splitIndex++] = str + strIndex;

		strIndex++;
	}

	return splitIndex;
}

char *trim(char *str) {
	size_t index = 0;
	char *trimmedStr;

	while (str[index] == ' ')
		str[index++] = '\0';

	trimmedStr = str + index;

	while (str[index++] != '\0')
		;
	index--;
	while (str[--index] == ' ')
		str[index] = '\0';

	return trimmedStr;
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
