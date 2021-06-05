#include <stdbool.h>
#include "keyboard.h"
#include "port.h"
#include "Layouts/Spanish.h"
#include "video.h"

static char lastChar = 0;

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

bool deadActive = false;
uint8_t lastDeadKey = 0;

uint8_t getShiftState() {
    return (shift ^ (caps_state == 1 || caps_state == 2)) | ctrl << 1 | alt << 2;
}

char translate(char from) {
    char *orig = deadKeyTables[lastDeadKey][0];
    char *dest = deadKeyTables[lastDeadKey][1];

    int i = 0;
    while (orig[i] != '\0' && orig[i] != from) i++;
    if (orig[i] == '\0')
        return from;
    return dest[i];
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
            uint8_t shiftState = getShiftState();
            bool newKey = false;
            if (deadActive) {
                lastChar = translate(charMap[key][shiftState]);
                deadActive = false;
                newKey = true;
            } else {
                if (isDeadKey[key][shiftState]) {
                    deadActive = true;
                    lastDeadKey = charMap[key][shiftState];
                } else {
                    lastChar = charMap[key][shiftState];
                    newKey = true;
                }
            }
            if (newKey) {
                printChar(lastChar);
            }
        }
    }
}

uint8_t getLastChar() {
    return lastChar;
}