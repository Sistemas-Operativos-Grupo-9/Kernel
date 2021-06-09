#include <stdbool.h>
#include "keyboard.h"
#include "port.h"
#include "Layouts/Latin American.h"
#include "video.h"
#include "keys.h"
#include "process.h"
#include "views.h"

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
#define K_F1 0x3B
#define K_F2 0x3C
#define K_F3 0x3D
#define K_F4 0x3E

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
    for (int i = 0; i < deadKeyTables.tablesCount; i++) {
        if (deadKeyTables.tables[i].deadCode == lastDeadKey) {
            for (int c = 0; c < deadKeyTables.tables[i].translationCount; c++) {
                if (deadKeyTables.tables[i].translations[c].from == from) {
                    return deadKeyTables.tables[i].translations[c].to;
                }
            }
        }
    }
    return from;
}

void sendChar(char c) {
    writeChar(getFocusedProcess().tty, c);
}
void sendEOF() {
    getFocusedProcess().fdTable->eof = true;
}

extern bool eof;

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
    } else if (key == K_BACKSPACE && pressed) {
        sendChar('\b');
    } else if ((key == K_F1 || key == K_F2 || key == K_F3 || key == K_F4) && pressed) {
        if (key == K_F1) {
            setFocus(0);
        } else if (key == K_F2) {
            setFocus(1);
        }
        // sendChar(ESC);
        // sendChar('O');
        // sendChar(code);
        deadActive = false;
    } else {
        if (pressed) {
            uint8_t shiftState = getShiftState();
            char ch;
            bool newKey = false;
            if (deadActive) {
                ch = translate(charMap[key][shiftState]);
                deadActive = false;
                newKey = true;
            } else {
                if (isDeadKey[key][shiftState]) {
                    deadActive = true;
                    lastDeadKey = charMap[key][shiftState];
                } else {
                    ch = charMap[key][shiftState];
                    newKey = true;
                }
            }
            if (newKey) {
                if ((ch == 'd' || ch == 'D') && ctrl) {
                    sendEOF();
                } else {
                    sendChar(ch);
                }
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

    // If escape is detected
    if (keyBuffer[0] == 0xE0) {
        if (keyBufferSize < 2)
            return; // We need more data to determine the key
        uint8_t code = keyBuffer[1];
        code &= ~0x80;
        keyBufferSize = 0;


        if ((code == K_UP || code == K_DOWN || code == K_RIGHT || code == K_LEFT) && pressed) {
            sendChar(ESC);
            sendChar(BRACKET);
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