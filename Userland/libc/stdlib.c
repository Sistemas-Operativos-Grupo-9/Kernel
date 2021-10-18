#include "stdlib.h"
#include "stdbool.h"

bool isUppercase(char ch) { return ch >= 'A' && ch <= 'Z'; }
bool isLowercase(char ch) { return ch >= 'a' && ch <= 'z'; }

uint8_t chartodig(char ch) {
	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (isUppercase(ch))
		return ch - 'A';
	if (isLowercase(ch))
		return ch - 'a';

	return 255;
}

uint64_t strtonum(char *num, uint8_t base) {
	uint64_t out = 0;
	if (*num == '\0')
		return -1;
	do {
		out *= base;
		out += chartodig(*num);
		num++;
	} while (*num != '\0');

	return out;
}