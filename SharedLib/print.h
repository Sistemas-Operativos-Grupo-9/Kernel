
#include "stdint.h"

int64_t write(uint64_t fd, const char *str, uint64_t size);

void putchar(char ch);
void puts(const char *str);
void printIntN(int value, uint8_t digits, uint8_t base);
void printInt(int value, uint8_t base);
void printUnsignedN(uint64_t value, uint8_t digits, uint8_t base);
void printUnsigned(uint64_t value, uint8_t base);
void printHexPrefix();
void printHexByte(uint8_t value);
void printHexPointer(void *ptr);
void printPercentage(float value);
