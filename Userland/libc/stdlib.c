#include "stdlib.h"

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

bool strtonum(char *num, uint64_t *out, uint8_t base) {
	*out = 0;
	if (*num == '\0')
		return false;
	do {
		*out *= base;
		*out += chartodig(*num);
		num++;
	} while (*num != '\0');

	return true;
}