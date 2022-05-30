#include "print.h"
#include "myUtils.h"
#include "stdbool.h"

void putchar_generic(int fd, char ch) { write(fd, &ch, 1); }

void puts_generic(int fd, const char *str) {
	while (*str != '\0') {
		putchar_generic(fd, *str);
		str++;
	}
}

void printIntN_generic(int fd, int value, uint8_t digits, uint8_t base) {
	bool neg = value < 0;
	char str[digits + 1 + (neg ? 1 : 0)];
	numToString(value, digits + (neg ? 1 : 0), str, base);
	puts_generic(fd, str);
}
void printInt_generic(int fd, int value, uint8_t base) {
	printIntN_generic(fd, value, countDigits(value, base), base);
}

void printUnsignedN_generic(int fd, uint64_t value, uint8_t digits, uint8_t base) {
	char str[digits + 1];
	unsignedToString(value, digits, str, base);
	puts_generic(fd, str);
}

void printUnsigned_generic(int fd, uint64_t value, uint8_t base) {
	printUnsignedN_generic(fd, value, countDigits(value, base), base);
}

void printHexPrefix_generic(int fd) {
	putchar_generic(fd, '0');
	putchar_generic(fd, 'x');
}

void printHexByte_generic(int fd, uint8_t value) {
	printHexPrefix_generic(fd);
	printUnsignedN_generic(fd, value, 2, 16);
}

void printHexPointer_generic(int fd, void *ptr) {
	printHexPrefix_generic(fd);
	/*printUnsignedN_generic(fd, (uint64_t)ptr, 16, 16);*/
	printUnsigned_generic(fd, (uint64_t)ptr, 16);
}

void printPercentage_generic(int fd, float value) {
	putchar_generic(fd, '\r');
	if (value >= 1) {
		puts_generic(fd, "100");
	} else if (value < 0) {
		puts_generic(fd, "0");
	} else {
		printUnsigned_generic(fd, (uint8_t)(value * 100), 10);
	}
	putchar_generic(fd, '%');
}

