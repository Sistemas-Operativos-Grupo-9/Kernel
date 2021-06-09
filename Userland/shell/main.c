#include <stdio.h>


int main() {
    puts(">");
    // while ()
    
    KeyStroke key;
    do {
        key = readKeyStroke();
        if (key.isPrintable) {
            putchar(key.data);
        }
    } while (false);
    // } while (!key.isEOF);
    // while (true) {
    //     putchar(getch());
    // }
    
    return 0;
}