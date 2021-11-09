#include "words.h"
#include <print.h>
#include <random.h>
#include <stdbool.h>
#include <stddef.h>
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
	setForeground(ORANGE);
	char ch[2] = {};
	int y = 320;
	int x = 10;
	drawText("Letras usadas: ", x, y, false);
	x += 180;
	setForeground(GREEN_YELLOW);
	for (int i = 0; i < state.usedLettersCount; i++) {
		ch[0] = state.usedLetters[i];
		drawText(ch, x + i * 20, y, false);
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
	setForeground(state.lives == 0 ? RED : GREEN);
	drawText(message, 250, 200, false);
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

void _start();
void setupWords() {
	for (int i = 0; i < sizeof(words) / sizeof(*words); i++) {
		words[i] += (uint64_t)_start;
	}
}

int main() {
	setupWords();
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