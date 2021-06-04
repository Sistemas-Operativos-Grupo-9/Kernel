#include <stdbool.h>
#include "keyboard.h"
#include "port.h"
#include "Layouts/US.h"

static unsigned long ticks = 0;
static char lastChar = 0;
// char *chars = "  1234567890-=  qwertyuiop[]  asdfghjkl;'`\\zxcvbnm,./ *"

void keyboard_handler() {
    // print("Key");
    uint8_t key = in(0x60);
    bool pressed = key >> 7 == 0;
    if (pressed) {
        key &= ~0x80;
        lastChar = charMap[key][0];
    }
    ticks++;
}

unsigned long getPresses() {
    return ticks;
}

uint8_t getLastChar() {
    return lastChar;
}