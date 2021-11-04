#include "words.h"
#include <null.h>
#include <print.h>
#include <random.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>

struct State {
	int lives;
	const char *word;
	bool revealed[MAX_WORD_LENGTH];
	char usedLetters[64];
	int usedLettersCount;
} state = {
    .lives = 6,
    .word = NULL,
    .usedLetters = {0},
    .usedLettersCount = 0,
    .revealed = {0},
};

void drawHanger(uint16_t x, uint16_t y) {
	setForeground(state.lives == 0 ? RED : WHITE);
	drawLine(x - 20, y, x + 100, y);
	drawLine(x, y, x, y - 270);
	y -= 270;
	drawLine(x - 20, y, x + 100, y);
	drawLine(x, y + 30, x + 30, y);
	x += 100;
	drawLine(x, y, x, y + 40);
}
void drawStickman(uint16_t x, uint16_t y, int lives) {
	if (lives > 5)
		return;
	// Draw head
	setForeground(lives == 0 ? RED : WHITE);
	drawCircle(x, y, 26);
	setForeground(BLACK);
	drawCircle(x, y, 24);
	y += 26;

	// Draw body
	setForeground(lives == 0 ? RED : WHITE);
	if (lives > 4)
		return;
	drawLine(x, y, x, y + 80);
	// Left arm
	if (lives > 3)
		return;
	drawLine(x, y + 20, x - 40, y + 50);
	// Right arm
	if (lives > 2)
		return;
	drawLine(x, y + 20, x + 40, y + 50);

	y += 80;
	// Left leg
	if (lives > 1)
		return;
	drawLine(x, y, x - 30, y + 60);
	// Right leg
	if (lives > 0)
		return;
	drawLine(x, y, x + 30, y + 60);
}

bool didPlayerWin() {
	for (int i = 0; state.word[i] != '\0'; i++) {
		if (!state.revealed[i])
			return false;
	}
	return true;
}

void drawWord() {
	char redacted[MAX_WORD_LENGTH];
	strcpy(redacted, state.word);

	for (int i = 0; state.word[i] != '\0'; i++)
		if (!state.revealed[i])
			redacted[i] = '_';

	setForeground(WHITE);
	drawText(redacted, 300, 150, false);
}

void drawUsedLetters() {
	setForeground(WHITE);
	char ch[2] = {};
	int y = 300;
	drawText("Letras usadas: ", 300, y, false);
	for (int i = 0; i < state.usedLettersCount; i++) {
		ch[0] = state.usedLetters[i];
		drawText(ch, 480 + i * 20, y, false);
	}
}

void draw() {
	WindowInfo winInfo = getWindowInfo();
	setForeground(BLACK);
	drawRectangle(0, 0, winInfo.pixelWidth, winInfo.pixelHeight);
	drawHanger(100, 300);
	drawStickman(200, 100, state.lives);
	drawWord();
	drawUsedLetters();

	char *message = didPlayerWin()     ? "GANASTE!"
	                : state.lives == 0 ? "PERDISTE!"
	                                   : "";
	setForeground(WHITE);
	drawText(message, 500, 150, false);
	flip();
}

#define TOUPPER(c) (c >= 'a' ? c - ('a' - 'A') : c)

bool revealLetters(char letter) {
	bool revealed = false;
	for (int i = 0; state.word[i] != '\0'; i++) {
		if (state.word[i] == letter) {
			revealed = true;
			state.revealed[i] = true;
		}
	}
	return revealed;
}

bool wasLetterUsed(char letter) {
	for (int i = 0; i < state.usedLettersCount; i++) {
		if (state.usedLetters[i] == letter) {
			return true;
		}
	}
	return false;
}

void guessLetter(char letter) {
	if (wasLetterUsed(letter)) {
		return;
	}

	state.usedLetters[state.usedLettersCount++] = letter;

	if (!revealLetters(letter)) {
		state.lives--;
	}
}

int main() {
	setGraphic(true);
	srand(millis());
	int random = rand() % (sizeof(words) / sizeof(*words));
	state.word = words[random];
	KeyStroke key;
	do {
		draw();
		key = readKeyStroke(0);
		if (key.isPrintable) {
			char c = TOUPPER(key.data);
			if (c >= 'A' && c <= 'Z')
				guessLetter(c);
		}
	} while (state.lives > 0 && !didPlayerWin());

	draw();
	millisleep(1000);

	setGraphic(false);

	return 0;
}