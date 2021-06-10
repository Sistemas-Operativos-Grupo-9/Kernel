#include "print.h"
#include "myUtils.h"
#include "stdio.h"

void printIntN(int value, uint8_t digits, uint8_t base) {
    char str[digits + 1];
    numToString(value, digits, str, base);
    puts(str);
}
void printInt(int value, uint8_t base) {
    printIntN(value, countDigits(value, base), base);
}

void printUnsignedN(unsigned value, uint8_t digits, uint8_t base) {
    char str[digits + 1];
    unsignedToString(value, digits, str, base);
    puts(str);
}

void printUnsigned(unsigned value, uint8_t base) {
    printUnsignedN(value, countDigits(value, base), base);
}

void printHexPrefix() {
    putchar('0');
    putchar('x');
}

void printHexByte(uint8_t value) {
    printHexPrefix();
    printUnsignedN(value, 2, 16);
}

void printHexPointer(void *ptr) {
    printHexPrefix();
    printUnsignedN((uint64_t) ptr, 16, 16);
}