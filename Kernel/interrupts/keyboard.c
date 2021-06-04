#include <stdbool.h>
#include "keyboard.h"
#include "port.h"
#include "Layouts/US.h"
#include "video.h"

static unsigned long ticks = 0;
static char lastChar = 0;
// char *chars = "  1234567890-=  qwertyuiop[]  asdfghjkl;'`\\zxcvbnm,./ *"

#define K_LSHIFT 0x2A
#define K_RSHIFT 0x36
#define K_BACKSPACE 0x0E
#define K_CONTROL 0x1D
#define K_TAB 0x0F
#define K_CAPS 0x3A

uint8_t caps_state = 0;
bool shift = false;
bool ctrl = false;
bool alt = false;

uint8_t getShiftState() {
    return (shift ^ (caps_state == 1 || caps_state == 2)) | ctrl << 1 | alt << 2;
}

void keyboard_handler() {
    uint8_t key = in(0x60);
    bool pressed = key >> 7 == 0;
    key &= ~0x80;
    if (key == K_LSHIFT || key == K_RSHIFT) {
        shift = pressed;
    } else if (key == K_CAPS) {
        caps_state++;
        if (caps_state > 3)
            caps_state = 0;
    } else if (key == K_CONTROL) {
        ctrl = pressed;
    } else {
        if (pressed) {
            lastChar = charMap[key][getShiftState()];
            printChar(lastChar);
        }
    }
}

uint8_t getLastChar() {
    return lastChar;
}