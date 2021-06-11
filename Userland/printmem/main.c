#include <stdio.h>
#include <stdlib.h>
#include <print.h>

int main(char **argv, int argc) {
    if (argc != 1) {
        puts("Usage: printmem <pointer (without 0x)>\n");
        return -1;
    }
    
    uint64_t *pointer = (uint64_t *)strtonum(argv[0], 16);
    printHexPointer(pointer);
    puts(":\n");


    for (int i = 0; i < 4; i++) {
        puts("    ");
        printHexPointer((void *)(*pointer++));
        putchar('\n');
    }

    return 0;
}