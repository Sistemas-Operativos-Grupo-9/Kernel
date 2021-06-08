
#include <stdint.h>
#include "video.h"
#include "keyboardBuffer.h"
#include "syscalls.h"


int64_t read(uint64_t fd, char *buf, uint64_t count) {
    int read;
    for (read = 0; read < count; read++) {
        // while (getAvailable() == 0);
        if (getAvailable() == 0)
            break;
        buf[read] = readChar();
    }
    return read;
}


int64_t write(uint64_t fd, char *buf, uint64_t count) {
    int i;
    for (i = 0; i < count; i++) {
        printChar(buf[i]);
    }
    return i;
}


uint64_t syscallDispatcher(uint64_t rdi, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5) {
    switch (rdi) {
        case READ: // drawChar: char, x, y
            return read(param1, (char *)param2, param3);
        case WRITE: // getChar
            return write(param1, (char *)param2, param3);
    }
    return 0;
}