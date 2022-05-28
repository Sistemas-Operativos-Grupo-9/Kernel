#include "myUtils.h"

char getDigit(uint8_t num) {
	if (num < 10)
		return num + '0';
	num -= 10;
	if (num < 26)
		return num + 'A';
	return num;
}

void numToString(int num, uint8_t digits, char out[], uint8_t base) {
	int i = 0;
	if (num < 0) {
		out[i++] = '-';
		num = -num;
	}

	unsignedToString(num, digits - i, out + i, base);
}

uint64_t pow(unsigned base, unsigned exponent) {
	uint64_t res = 1;
	for (int i = 0; i < exponent; i++) {
		res *= base;
	}
	return res;
}

uint8_t countDigits(unsigned num, uint8_t base) {
	uint8_t digits = 1;
	while (num >= base) {
		num /= base;
		digits++;
	}
	return digits;
}

void unsignedToString(uint64_t num, uint8_t digits, char out[], uint8_t base) {
	uint64_t div = pow(base, digits - 1);

	for (int i = 0; i < digits; i++) {
		out[i] = getDigit((num / div) % base);
		div /= base;
	}

	out[digits] = '\0';
}