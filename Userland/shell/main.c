#include <stdio.h>
#include <time.h>
#include <print.h>
#include <syscall.h>

int main() {
    puts(">");
    
    puts("PID: ");
    printUnsigned(getpid(), 10);
    putchar('\n');
    // for (int i = 0; i < 1; i++) {
    //     wait();
    // }
    // int i = 0;
    // while (true) {
        // printUnsigned(i++, 10);
    while (true) {
        putchar(getch());
        putchar(' ');
    }
    // }
    
    return 0;
}