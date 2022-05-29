
#include "stdint.h"

int64_t write(uint64_t fd, const char *str, uint64_t size);

void putchar_generic(int fd, char ch);
void puts_generic(int fd, const char *str);
void printIntN_generic(int fd, int value, uint8_t digits, uint8_t base);
void printInt_generic(int fd, int value, uint8_t base);
void printUnsignedN_generic(int fd, uint64_t value, uint8_t digits, uint8_t base);
void printUnsigned_generic(int fd, uint64_t value, uint8_t base);
void printHexPrefix_generic(int fd);
void printHexByte_generic(int fd, uint8_t value);
void printHexPointer_generic(int fd, void *ptr);
void printPercentage_generic(int fd, float value);

#define putchar(ch) putchar_generic(1, ch)
#define puts(str) puts_generic(1, str)
#define printIntN(value, digits, base) printIntN_generic(1, value, digits, base)
#define printInt(value, base) printInt_generic(1, value, base)
#define printUnsignedN(value, digits, base) printUnsignedN_generic(1, value, digits, base)
#define printUnsigned(value, base) printUnsigned_generic(1, value, base)
#define printHexPrefix() printHexPrefix_generic(1)
#define printHexByte(value) printHexByte_generic(1, value)
#define printHexPointer(ptr) printHexPointer_generic(1, ptr)
#define printPercentage(value) printPercentage_generic(1, value)

#define eputchar(ch) putchar_generic(2, ch)
#define eputs(str) puts_generic(2, str)
#define eprintIntN(value, digits, base) printIntN_generic(2, value, digits, base)
#define eprintInt(value, base) printInt_generic(2, value, base)
#define eprintUnsignedN(value, digits, base) printUnsignedN_generic(2, value, digits, base)
#define eprintUnsigned(value, base) printUnsigned_generic(2, value, base)
#define eprintHexPrefix() printHexPrefix_generic(2)
#define eprintHexByte(value) printHexByte_generic(2, value)
#define eprintHexPointer(ptr) printHexPointer_generic(2, ptr)
#define eprintPercentage(value) printPercentage_generic(2, value)
