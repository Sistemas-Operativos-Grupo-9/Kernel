#include <stdbool.h>
#include "keyboard.h"
#include "port.h"
#include "Layouts/Latin American.h"
#include "video.h"

static char lastChar = 0;

#define K_RETURN 0x1C
#define K_LSHIFT 0x2A
#define K_RSHIFT 0x36
#define K_BACKSPACE 0x0E
#define K_CONTROL 0x1D
#define K_TAB 0x0F
#define K_CAPS 0x3A

#define K_UP 0x48
#define K_DOWN 0x50
#define K_RIGHT 0x4D
#define K_LEFT 0x4B
#define K_ALT 0x38

uint8_t caps_state = 0;
bool shift = false;
bool ctrl = false;
bool alt = false;

bool deadActive = false;
uint8_t lastDeadKey = 0;

uint8_t keyBuffer[8];
int keyBufferSize = 0;

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

void sendChar(char c) {
    printChar(c);
}

void handleSingleByteKey(uint8_t key, bool pressed) {
    if (key == K_LSHIFT || key == K_RSHIFT) {
        shift = pressed;
    } else if (key == K_CAPS) {
        caps_state++;
        if (caps_state > 3)
            caps_state = 0;
    } else if (key == K_CONTROL) {
        ctrl = pressed;
    } else if (key == K_RETURN && pressed) {
        sendChar('\n');
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
                sendChar(lastChar);
            }
        }
    }
}

void keyboard_handler() {
    uint8_t key = in(0x60);
    keyBuffer[keyBufferSize++] = key;
    // printChar(' ');
    // printHexByte(key);
    
    // Process buffer
    bool pressed = key >> 7 == 0;

    // If escape for arrows is detected
    if (keyBuffer[0] == 0xE0) {
        if (keyBufferSize < 2)
            return; // We need more data to determine the key
        uint8_t code = keyBuffer[1];
        code &= ~0x80;
        keyBufferSize = 0;

        if ((code == K_UP || code == K_DOWN || code == K_RIGHT || code == K_LEFT) && pressed) {
            sendChar(0x1b);
            sendChar(0x5b);
            char arrowChar;
            switch (code) {
                case K_UP:
                    arrowChar = 'A';
                    break;
                case K_DOWN:
                    arrowChar = 'B';
                    break;
                case K_RIGHT:
                    arrowChar = 'C';
                    break;
                case K_LEFT:
                    arrowChar = 'D';
                    break;
            }
            sendChar(arrowChar);
            deadActive = false;
        } else if (code == K_ALT) {
            alt = pressed;
        } else {
            if (pressed) {
                if (code == K_RETURN) {
                    sendChar('\n');
                }
            }
        }
        return;
    }


    handleSingleByteKey(keyBuffer[0] & ~0x80, pressed);
    keyBufferSize = 0;
}

uint8_t getLastChar() {
    return lastChar;
}