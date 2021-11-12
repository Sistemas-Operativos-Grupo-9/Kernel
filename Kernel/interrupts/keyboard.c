#include "keyboard.h"
#include "Layouts/Latin American.h"
#include "interrupts.h"
#include "registers.h"
#include "keys.h"
#include "port.h"
#include "process.h"
#include "video.h"
#include "views.h"
#include <stdbool.h>

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

#define K_F5 0x3F
#define K_F6 0x40
#define K_F7 0x41
#define K_F8 0x42

#define K_F9 0x43
#define K_F10 0x44
#define K_F11 0x57
#define K_F12 0x58

#define K_KP7 0x47
#define K_KP8 0x48
#define K_KP9 0x49
#define K_KPMINUS 0x4A
#define K_KP4 0x4B
#define K_KP5 0x4C
#define K_KP6 0x4D
#define K_KPPLUS 0x4E
#define K_KP1 0x4F
#define K_KP2 0x50
#define K_KP3 0x51
#define K_KP0 0x52
#define K_KPDOT 0x53

#define K_PGUP 0x49
#define K_PGDN 0x51

uint8_t caps_state = 0;
bool shift = false;
bool ctrl = false;
bool left_alt = false;
bool right_alt = false;

bool deadActive = false;
uint8_t lastDeadKey = 0;

uint8_t keyBuffer[8];
int keyBufferSize = 0;

uint8_t getShiftState() {
	return (shift ^ (caps_state == 1 || caps_state == 2)) | ctrl << 1 |
	       (left_alt || right_alt) << 2;
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

void sendChar(char c) { writeChar(getFocusedProcess()->tty, c); }
void sendEOF() { getFocusedProcess()->fdTable->eof = true; }

extern bool eof;

void handleSingleByteKey(uint8_t key, bool pressed, struct RegistersState *registers) {
	if (key == K_LSHIFT || key == K_RSHIFT) {
		shift = pressed;
	} else if (key == K_CAPS) {
		caps_state++;
		if (caps_state > 3)
			caps_state = 0;
	} else if (key == K_CONTROL) {
		ctrl = pressed;
	} else if (key == K_ALT) {
		left_alt = pressed;
	} else if (key == K_RETURN && pressed) {
		sendChar('\n');
	} else if (key == K_BACKSPACE && pressed) {
		sendChar('\b');
	} else if ((key == K_KP0 || key == K_KP1 || key == K_KP2 || key == K_KP3 ||
	            key == K_KP4 || key == K_KP5 || key == K_KP6 || key == K_KP7 ||
	            key == K_KP8 || key == K_KP9) &&
	           pressed) {

		switch (key) {
		case K_KP0:
			sendChar('0');
			break;
		case K_KP1:
			sendChar('1');
			break;
		case K_KP2:
			sendChar('2');
			break;
		case K_KP3:
			sendChar('3');
			break;
		case K_KP4:
			sendChar('4');
			break;
		case K_KP5:
			sendChar('5');
			break;
		case K_KP6:
			sendChar('6');
			break;
		case K_KP7:
			sendChar('7');
			break;
		case K_KP8:
			sendChar('8');
			break;
		case K_KP9:
			sendChar('9');
			break;
		}
	} else if ((key == K_F1 || key == K_F2 || key == K_F3 || key == K_F4 ||
	            key == K_F5 || key == K_F6 || key == K_F7 || key == K_F8 ||
	            key == K_F9 || key == K_F10 || key == K_F11 || key == K_F12) &&
	           pressed) {
		// if (key == K_F1) {
		// 	setFocus(0);
		// } else if (key == K_F2) {
		// 	setFocus(1);
		// } else if (key == K_F3) {
		// 	setFocus(2);
		// } else if (key == K_F4) {
		// 	setFocus(3);
		// } else if (key == K_F5) {
		// 	setFocus(4);
		// } else if (key == K_F6) {
		// 	setFocus(5);
		// }
		if (key == K_F1) {
			focusDesktop(0);
		} else if (key == K_F2) {
			focusDesktop(1);
		}
		if (key == K_F12) {
			_storeRegisters(registers);
		}
		// sendChar(ESC);
		// sendChar('O');
		// sendChar(code);
		deadActive = false;
	} else if ((key == K_TAB) && pressed) {
		focusNextView();
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

void keyboard_handler(struct RegistersState *registers) {
	uint8_t key = in(0x60);
	keyBuffer[keyBufferSize++] = key;
	// putchar(0, ' ');
	// printHexByte(0, key);

	// Process buffer
	bool pressed = key >> 7 == 0;

	// If escape is detected
	if (keyBuffer[0] == 0xE0) {
		if (keyBufferSize < 2)
			return; // We need more data to determine the key
		uint8_t code = keyBuffer[1];
		code &= ~0x80;
		keyBufferSize = 0;

		if ((code == K_UP || code == K_DOWN || code == K_RIGHT ||
		     code == K_LEFT) &&
		    pressed) {
			sendChar(ESC);
			sendChar(BRACKET);
			char arrowChar = '?';
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
			right_alt = pressed;
		} else if ((code == K_PGUP || code == K_PGDN) && pressed) {
			uint8_t tty = getFocusedProcess()->tty;
			lookAround(tty, code == K_PGDN ? 1 : -1);

		} else {
			if (pressed) {
				if (code == K_RETURN) {
					sendChar('\n');
				}
			}
		}
		return;
	}

	handleSingleByteKey(keyBuffer[0] & ~0x80, pressed, registers);
	keyBufferSize = 0;
}
