#include <myUtils.h>

char getDigit(uint8_t num) {
    if (num < 10)
        return num + '0';
    num -= 10;
    if (num < 26)
        return num + 'A';
    return num;
}

void numToString(int num, uint8_t digits, char out[static digits + 1], uint8_t base) {
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

void unsignedToString(int num, uint8_t digits, char out[static digits + 1], uint8_t base) {
    long long unsigned div = pow(base, digits - 1);

    for (int i = 0; i < digits; i++) {
        out[i] = getDigit((num / div) % base);
        div /= base;
    }

    out[digits] = '\0';
}