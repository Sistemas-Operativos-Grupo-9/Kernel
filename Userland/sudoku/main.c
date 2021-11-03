
#include "sudokus.h"
#include <print.h>
#include <random.h>
#include <stdbool.h>
#include <stdio.h>
#include <syscall.h>

struct State {
	struct {
		int x, y;
	} cursor;
	uint8_t sudoku[9][9];
	uint8_t numbers[9][9];
} state = {
    .cursor =
        {
            .x = 4,
            .y = 4,
        },
    .numbers = {{0}},
    .sudoku = {{0}},
};

bool validateNumbers(uint8_t numbers[9]) {
	for (int i = 0; i < 9; i++) {
		uint8_t value = numbers[i];
		if (value != 0) {
			for (int j = i + 1; j < 9; j++) {
				if (value == numbers[j])
					return false;
			}
		}
	}
	return true;
}

uint8_t getFinalNumber(uint8_t x, uint8_t y) {
	return state.sudoku[y][x] ? state.sudoku[y][x] : state.numbers[y][x];
}
bool validateSquare(uint8_t sx, uint8_t sy) {
	uint8_t square[9];
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			square[y * 3 + x] = getFinalNumber(sx * 3 + x, sy * 3 + y);
		}
	}
	return validateNumbers(square);
}
bool validateSquares() {
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (!validateSquare(x, y))
				return false;
		}
	}

	return true;
}

bool validateHorizontalLine(uint8_t ly) {
	uint8_t line[9];
	for (int x = 0; x < 9; x++) {
		line[x] = getFinalNumber(x, ly);
	}
	return validateNumbers(line);
}
bool validateHorizontal() {
	for (int y = 0; y < 9; y++) {
		if (!validateHorizontalLine(y))
			return false;
	}
	return true;
}

bool validateVerticalLine(uint8_t lx) {
	uint8_t line[9];
	for (int y = 0; y < 9; y++) {
		line[y] = getFinalNumber(lx, y);
	}
	return validateNumbers(line);
}
bool validateVertical() {
	for (int x = 0; x < 9; x++) {
		if (!validateVerticalLine(x))
			return false;
	}
	return true;
}

bool isSudokuValid() {
	if (!validateHorizontal())
		return false;
	if (!validateVertical())
		return false;
	if (!validateSquares())
		return false;
	return true;
}
bool isComplete() {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			if (!getFinalNumber(x, y))
				return false;
		}
	}
	return true;
}

bool didPlayerWin() { return isComplete() && isSudokuValid(); }

#define SUDOKU_SQUARE_BORDER 3
#define SUDOKU_BORDER 1
#define SQUARE_SIDE 36
#define BIG_SQUARE_SIDE SQUARE_SIDE * 3 + SUDOKU_BORDER * 2
#define FULL_SIDE SQUARE_SIDE * 9 + SUDOKU_SQUARE_BORDER * 4 + SUDOKU_BORDER * 6

void drawSudoku() {
	bool valid = isSudokuValid();
	setForeground(valid ? GREY_N2 : RED);
	drawRectangle(0, 0, FULL_SIDE, FULL_SIDE);
	char c[2] = {0};

	for (int sy = 0; sy < 3; sy++) {
		for (int sx = 0; sx < 3; sx++) {
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					uint8_t finalX = sx * 3 + x;
					uint8_t finalY = sy * 3 + y;
					uint16_t px =
					    SUDOKU_SQUARE_BORDER +
					    sx * (BIG_SQUARE_SIDE + SUDOKU_SQUARE_BORDER) +
					    x * (SQUARE_SIDE + SUDOKU_BORDER);
					uint16_t py =
					    SUDOKU_SQUARE_BORDER +
					    sy * (BIG_SQUARE_SIDE + SUDOKU_SQUARE_BORDER) +
					    y * (SQUARE_SIDE + SUDOKU_BORDER);
					setForeground(BLACK);
					drawRectangle(px, py, SQUARE_SIDE, SQUARE_SIDE);
					bool isCursor =
					    state.cursor.x == finalX && state.cursor.y == finalY;
					if (isCursor) {
						setForeground(GREEN);
						drawRectangle(px, py, SQUARE_SIDE, SQUARE_SIDE);
						setForeground(BLACK);
						drawRectangle(px + SUDOKU_BORDER, py + SUDOKU_BORDER,
						              SQUARE_SIDE - SUDOKU_BORDER * 2,
						              SQUARE_SIDE - SUDOKU_BORDER * 2);
					}
					uint8_t n = getFinalNumber(finalX, finalY);
					bool playerNumber = state.numbers[finalY][finalX];
					if (n) {
						c[0] = n + '0';
						setForeground(playerNumber ? GREEN_YELLOW : WHITE);
						drawText(c, px + SQUARE_SIDE / 2, py + SQUARE_SIDE / 2,
						         true);
					}
				}
			}
		}
	}
	flip();
	// for (int y = 0; y < 9; y++) {
	// 	if (y % 3 == 0) {
	// 		for (int i = 0; i < 13; i++)
	// 			putchar('-');
	// 		putchar('\n');
	// 	}
	// 	for (int x = 0; x < 9; x++) {
	// 		uint8_t value = state.sudoku[y][x];
	// 		uint8_t userValue = state.numbers[y][x];
	// 		if (x % 3 == 0)
	// 			putchar('|');
	// 		bool isCursor = state.cursor.x == x && state.cursor.y == y;
	// 		if (!valid)
	// 			setForeground(RED);
	// 		else if (isCursor)
	// 			setForeground(ORANGE);
	// 		else if (userValue != 0)
	// 			setForeground(GREEN);

	// 		if (userValue != 0) {
	// 			printUnsignedN(userValue, 1, 10);
	// 		} else if (value == 0) {
	// 			putchar(isCursor ? '#' : ' ');
	// 		} else {
	// 			printUnsignedN(value, 1, 10);
	// 		}
	// 		setForeground(WHITE);
	// 	}
	// 	putchar('|');
	// 	putchar('\n');
	// }
	// for (int i = 0; i < 13; i++)
	// 	putchar('-');
	// putchar('\n');
}

bool isDigit(char digit) { return digit >= '0' && digit <= '9'; }
uint8_t charToDigit(char digit) { return digit - '0'; }

int main() {
	setGraphic(true);
	srand(millis());
	int random = rand() % 128;
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			state.sudoku[y][x] = sudokus[random][y][x];
		}
	}
	KeyStroke key;
	drawSudoku();
	do {
		key = readKeyStroke(0);
		if (key.arrow != NO_ARROW) {
			switch (key.arrow) {
			case ARROW_LEFT:
				if (state.cursor.x > 0)
					state.cursor.x--;
				break;
			case ARROW_RIGHT:
				if (state.cursor.x < 8)
					state.cursor.x++;
				break;
			case ARROW_UP:
				if (state.cursor.y > 0)
					state.cursor.y--;
				break;
			case ARROW_DOWN:
				if (state.cursor.y < 8)
					state.cursor.y++;
				break;
			}
		} else if (key.isPrintable && isDigit(key.data)) {
			uint8_t digit = charToDigit(key.data);
			if (state.sudoku[state.cursor.y][state.cursor.x])
				continue;
			state.numbers[state.cursor.y][state.cursor.x] = digit;
		}
		drawSudoku();
	} while (!didPlayerWin() && key.data != 'q');
	setGraphic(false);
	puts(didPlayerWin() ? "GANASTE!\n" : "\n");
	return 0;
}