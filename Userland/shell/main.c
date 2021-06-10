#include <stdio.h>
#include <time.h>
#include <print.h>
#include <syscall.h>

int main() {
    puts("> HOLA DESDE SHELL\n");
    
    int i = 0;
    while (true) {
        printUnsigned(i++, 10);
        putchar('\n');
    }
    
    return 0;
}