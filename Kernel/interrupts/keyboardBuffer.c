#include "keyboardBuffer.h"
#include "lib.h"

char buffer[265];
uint8_t count = 0;

void writeChar(char ch) {
    buffer[count++] = ch;
}

char readChar() {
    char ret = *buffer;
    memcpy(buffer, buffer + 1, count - 1);
    count--;
    return ret;
}

uint8_t getAvailable() {
    return count;
}