#include "keyboard.h"
#include "port.h"

static unsigned long ticks = 0;
static uint8_t lastKeyCode = 0;

void keyboard_handler() {
    // print("Key");
    lastKeyCode = in(0x60);
    ticks++;
}

unsigned long getPresses() {
    return ticks;
}

uint8_t getLastKey() {
    return lastKeyCode;
}