#include "stdlib.h"

static bool isUppercase(char ch) { return ch >= 'A' && ch <= 'Z'; }
static bool isLowercase(char ch) { return ch >= 'a' && ch <= 'z'; }

static uint8_t chartodig(char ch) {
	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (isUppercase(ch))
		return ch - 'A';
	if (isLowercase(ch))
		return ch - 'a';

	return 255;
}

bool strtonum(const char *num, uint64_t *out, uint8_t base) {
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

bool strtoint(const char *num, int64_t *out, uint8_t base) {
	*out = 0;
	if (*num == '\0')
		return false;
	bool neg = false;
	if (*num == '-') {
		neg = true;
		num++;
	}
	uint64_t abs;
	bool ret = strtonum(num, &abs, base);
	if (!ret) {
		return ret;
	}

	*out = (int64_t)abs;
	if (neg) {
		*out = -*out;
	}

	return true;
}
