
#include "string.h"


uint64_t strlen(char *str) {
    char *end = str;
    while (*end++ != '\0');
    return end - str - 1;
}

void strcpy(char *dst, char *src) {
    do {
        *dst++ = *src++;
    } while (*(dst - 1) != '\0');
}