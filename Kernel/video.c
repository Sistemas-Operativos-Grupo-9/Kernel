#include <lib.h>
#include <myUtils.h>
#include <stdbool.h>
#include <video.h>

#include "basicVideo.h"
#include "keys.h"
#include "process.h"

struct TextColors {
	Color foreground;
	Color background;
} __attribute__((packed)); // currentColor = {.background = {0, 0, 0},
                           // .foreground = {255, 255, 255}};

#define NORMAL_COLORS                                                          \
	(struct TextColors) { .background = BLACK, .foreground = WHITE }

#define TEXT_WIDTH 83
#define TEXT_HEIGHT 29

#define TEXT_BUFFER_WIDTH TEXT_WIDTH
#define TEXT_BUFFER_HEIGHT TEXT_HEIGHT * 5

#define VIEWCLIP                                                               \
	getOffsetX() + (view->positionX * getFontWidth()),                         \
	    getOffsetY() + (view->positionY * getFontHeight()),                    \
	    view->width *getFontWidth(), view->height *getFontHeight()

static struct View {
	int cursorX;
	int cursorY;
	int scrollY; // First line being shown from textBuffer
	struct TextColors colors;
	char textBuffer[TEXT_BUFFER_HEIGHT][TEXT_BUFFER_WIDTH];
	char outputBuffer[256];
	uint32_t outputLength;
	uint32_t positionX, positionY;
	uint32_t width, height;
	bool graphic;
} Views[] = {
    {.positionX = 0,
     .positionY = 0,
     .width = TEXT_WIDTH,
     .height = TEXT_HEIGHT / 2},
    {.positionX = 0,
     .positionY = TEXT_HEIGHT / 2 + 1,
     .width = TEXT_WIDTH,
     .height = TEXT_HEIGHT / 2},
    // { .positionX = 0, .positionY = TEXT_HEIGHT / 2 + 1,
    // .width = TEXT_WIDTH / 2, .height = TEXT_HEIGHT / 2},
    // { .positionX = 0, .positionY = 0,
    // .width = TEXT_WIDTH / 2, .height = TEXT_HEIGHT},
    // { .positionX = TEXT_WIDTH / 2 + 1, .positionY = 0,
    // .width = TEXT_WIDTH / 2, .height = TEXT_HEIGHT / 2},
    // { .positionX = TEXT_WIDTH / 2 + 1, .positionY = TEXT_HEIGHT / 2 + 1,
    // .width = TEXT_WIDTH / 2, .height = TEXT_HEIGHT / 2}
};

uint8_t focusedView = 0;

uint32_t abs(int32_t n) {
	if (n < 0)
		return -n;
	return n;
}
uint16_t sqrt(uint32_t n) {
	uint16_t s;
	for (s = 0; s * s < n; s++)
		;
	return s;
}

// Color circle(uint64_t x, uint64_t y) {
// 	x -= getWidth() / 2;
// 	y -= getHeight() / 2;
// 	if (x * x + y * y < 150000) {
// 		return (Color) { x, y, 100 };
// 	}
// 	return GREY;
// }

// Generates an image with dots
Color backgroundImage(uint64_t x, uint64_t y) {
	const uint64_t lightDistances = 64;
	// Color colors[] = {BLUE, RED, GREEN};
	Color colors[] = {AQUA, LIGHT_SALMON, GREEN_YELLOW};
	int colorIndex = (x + lightDistances / 2) / lightDistances +
	                 (y + lightDistances / 2) / lightDistances;
	int closestX =
	    (x + lightDistances / 2) % lightDistances - lightDistances / 2;
	int closestY =
	    (y + lightDistances / 2) % lightDistances - lightDistances / 2;
	uint32_t closestLight =
	    sqrt(abs(closestX) * abs(closestX) + abs(closestY) * abs(closestY)) * 5;

	return colorLerp(colors[colorIndex % (sizeof(colors) / sizeof(*colors))],
	                 BLACK, closestLight);
	// uint64_t kernel[5][5] = {
	// 	{ 1, 1, 1, 1, 1 },
	// 	{ 1, 1, 1, 1, 1 },
	// 	{ 1, 1, 1, 1, 1 },
	// 	{ 1, 1, 1, 1, 1 },
	// 	{ 1, 1, 1, 1, 1 },
	// };
	// uint64_t kernelSum = 1*5*5;
	// uint64_t acum_red = 0;
	// uint64_t acum_green = 0;
	// uint64_t acum_blue = 0;
	// for (int iy = 0; iy < 5; iy++) {
	// 	for (int ix = 0; ix < 5; ix++) {
	// 		Color a = circle(x + ix - 2, y + iy - 2);
	// 		acum_red += kernel[iy][ix] * a.red;
	// 		acum_green += kernel[iy][ix] * a.green;
	// 		acum_blue += kernel[iy][ix] * a.blue;
	// 	}
	// }
	// acum_red /= kernelSum;
	// acum_green /= kernelSum;
	// acum_blue /= kernelSum;
	// return (Color) { acum_red, acum_green, acum_blue };
}

void initScreen() {
	initVideo();
	setCharOffset(TEXT_WIDTH, TEXT_HEIGHT);

	for (int i = 0; i < sizeof(Views) / sizeof(*Views); i++) {
		Views[i].cursorX = 0;
		Views[i].cursorY = 0;
		Views[i].outputLength = 0;
		Views[i].scrollY = 0;
		Views[i].colors = NORMAL_COLORS;
	}
	// for (int y = 0; y < TEXT_HEIGHT; y++) {
	//     for (int x = 0; x < TEXT_WIDTH; x++) {
	//         drawCharAt('*', x, y, BLACK, BLUE);
	//     }
	// }
	// drawRectangleRaw(0, 0, getWidth(), getHeight(), (Color) {200, 100, 50});
	drawImage(backgroundImage);

	for (int i = 0; i < sizeof(Views) / sizeof(*Views); i++) {
		struct View *view = Views + i;
		drawRectangleBorders(getOffsetX() + (view->positionX * getFontWidth()),
		                     getOffsetY() + (view->positionY * getFontHeight()),
		                     view->width * getFontWidth(),
		                     view->height * getFontHeight(), 3, GREY);
	}
}

void setForeground(uint8_t viewNumber, Color color) {
	(&Views[viewNumber])->colors.foreground = color;
}
void setBackground(uint8_t viewNumber, Color color) {
	(&Views[viewNumber])->colors.background = color;
}

void drawCharAtView(struct View *view, char ch, uint8_t x, uint8_t y,
                    Color background, Color foreground) {
	if (!view->graphic)
		if (x >= 0 && x < view->width && y >= 0 && y < view->height)
			drawCharAt(ch, x + view->positionX, y + view->positionY, background,
			           foreground);
}
Color invertColor(Color color) {
	return (Color){.red = 255 - color.red,
	               .green = 255 - color.green,
	               .blue = 255 - color.blue};
}
void redrawCharInverted(struct View *view, uint32_t x, uint32_t y) {
	Color foreground = view->colors.foreground;
	if (view - Views != focusedView)
		foreground =
		    colorLerp(view->colors.background, view->colors.foreground, 64);
	drawCharAtView(view, view->textBuffer[y][x], x, y - view->scrollY,
	               foreground, view->colors.background);
}
void redrawChar(struct View *view, uint32_t x, uint32_t y) {
	drawCharAtView(view, view->textBuffer[y][x], x, y - view->scrollY,
	               view->colors.background, view->colors.foreground);
}

void consume(struct View *view, int count) {
	view->outputLength -= count;
	for (int i = 0; i < view->outputLength; i++) {
		view->outputBuffer[i] = view->outputBuffer[i + count];
	}
}

bool isPrintable(unsigned char ch) {
	return ((ch >= 0x20 && ch <= 0x7E) || (ch >= 0x82 && ch <= 0x8C) ||
	        (ch >= 0x91 && ch <= 0x9C) || (ch == 0x9F) || (ch >= 0xA1));
}

void changeFocusView(uint8_t newFocusViewNumber) {
	focusedView = newFocusViewNumber;
	// Redraw cursors
	for (int i = 0; i < sizeof(Views) / sizeof(*Views); i++) {
		redrawCharInverted(&Views[i], Views[i].cursorX, Views[i].cursorY);
	}
}
void clearGraphic(struct View *view) {
	drawRectangleRaw(getOffsetX() + (view->positionX * getFontWidth()),
	                 getOffsetY() + (view->positionY * getFontHeight()),
	                 view->width * getFontWidth(),
	                 view->height * getFontHeight(), BLACK, VIEWCLIP);
}
void reDraw(struct View *view) {
	if (view->graphic) {
		clearGraphic(view);
	} else {
		for (int y = 0; y < view->height; y++) {
			for (int x = 0; x < view->width; x++) {
				redrawChar(view, x, y + view->scrollY);
			}
		}
		if (view->cursorY < view->scrollY + view->height &&
		    view->cursorY >= view->scrollY)
			redrawCharInverted(view, view->cursorX, view->cursorY);
	}
}
void lookAround(uint8_t viewNumber, int deltaY) {
	struct View *view = &Views[viewNumber];
	if (deltaY > 0) {
		int objY = view->scrollY + view->height + deltaY - 1;
		if (objY >= TEXT_BUFFER_HEIGHT) {
			objY = TEXT_BUFFER_HEIGHT - 1;
		}
		scrollTo(viewNumber, objY);
	} else if (deltaY < 0) {
		int objY = view->scrollY + deltaY;
		if (objY < 0) {
			objY = 0;
		}
		scrollTo(viewNumber, objY);
	}
}
int scrollTo(uint8_t viewNumber, int y) {
	struct View *view = &Views[viewNumber];

	while (y >= TEXT_BUFFER_HEIGHT) {
		int move = 10;
		int newY = y - move;
		int l = 0;
		for (; l < TEXT_BUFFER_HEIGHT; l++) {
			int from = l + move, to = l;
			for (int i = 0; i < TEXT_BUFFER_WIDTH; i++) {
				view->textBuffer[to][i] = from < TEXT_BUFFER_HEIGHT
				                              ? view->textBuffer[from][i]
				                              : '\0';
			}
		}
		y = newY;
		view->scrollY -= move;
	}
	if (y < 0)
		y = 0;
	if (y >= view->scrollY + view->height) {
		view->scrollY = y - view->height + 1;
		reDraw(view);
	} else if (y < view->scrollY) {
		view->scrollY = y;
		reDraw(view);
	}

	return y;
}

uint64_t countCharsAfter(struct View *view, int xFrom, int yFrom) {
	int length = 0;
	for (int y = yFrom;; y++) {
		for (int x = y == yFrom ? xFrom : 0; x < view->width; x++) {
			if (view->textBuffer[y][x] == '\0')
				return length;
			length++;
		}
	}
}

uint64_t umod(int n, uint64_t d) {
	n %= (int)d;
	if (n < 0)
		n += d;
	return n;
}
int idiv(int n, int d) { return (n - (int)umod(n, d)) / d; }
void moveFollowing(uint8_t viewNumber, int xFrom, int yFrom, int count) {
	if (count == 0)
		return;

	struct View *view = &Views[viewNumber];
#define getXfor(startX, length) umod(startX + length, view->width)
#define getYfor(startX, startY, length)                                        \
	(startY + idiv(startX + length, (int)view->width))

	int xTo = getXfor(xFrom, count);
	int yTo = getYfor(xFrom, yFrom, count);

	int totalLength = countCharsAfter(view, xFrom, yFrom) + 1;

	// Move from (from + i) -> (to + i)
	for (int length = 0; length < totalLength; length++) {
		int l = count > 0 ? totalLength - length - 1 : length;
		int yToI = getYfor(xTo, yTo, l), xToI = getXfor(xTo, l);
		view->textBuffer[yToI][xToI] =
		    view->textBuffer[getYfor(xFrom, yFrom, l)][getXfor(xFrom, l)];
		redrawChar(view, xToI, yToI);
	}

	// Clear between "from" and "to"
	if (count < 0) {
		int oldXTo = xTo;
		xTo = getXfor(oldXTo, totalLength);
		yTo = getYfor(oldXTo, yTo, totalLength);
		int oldXFrom = xFrom;
		xFrom = getXfor(oldXFrom, totalLength);
		yFrom = getYfor(oldXFrom, yFrom, totalLength);

		int tmp;
		tmp = xTo;
		xTo = xFrom;
		xFrom = tmp;

		tmp = yTo;
		yTo = yFrom;
		yFrom = tmp;
	}
	for (int y = yFrom; y <= yTo; y++) {
		for (int x = y == yFrom ? xFrom : 0;
		     y == yTo ? x < xTo : x < view->width; x++) {
			view->textBuffer[y][x] = '\0';
			redrawChar(view, x, y);
		}
	}

#undef getXfor
#undef getYfor
}

void setChar(uint8_t viewNumber, char ch) {
	struct View *view = &Views[viewNumber];
	moveFollowing(viewNumber, view->cursorX, view->cursorY, 1);
	view->textBuffer[view->cursorY][view->cursorX] = ch;
	redrawChar(view, view->cursorX, view->cursorY);
}

void printChar(uint8_t viewNumber, char ch) {
	struct View *view = &Views[viewNumber];
	view->outputBuffer[view->outputLength++] = ch;

	int newCursorX = view->cursorX, newCursorY = view->cursorY;

	bool finish = false;
	while (!finish && view->outputLength > 0) {
		finish = true;
		if (view->outputBuffer[0] == ESC) {
			if (view->outputLength > 1) {
				if (view->outputBuffer[1] == BRACKET) {
					if (view->outputLength > 2) {
						enum Arrow arrow = view->outputBuffer[2];
						switch (arrow) {
						case ARROW_LEFT:
							newCursorX--;
							break;
						case ARROW_RIGHT:
							newCursorX++;
							break;
						default:
							break;
						}
						consume(view, 3);
						finish = false;
					}
				} else {
					view->outputBuffer[0] = '?';
					finish = false;
				}
			}
		} else if (view->outputBuffer[0] == '\n') {
			newCursorX = 0;
			newCursorY++;
			consume(view, 1);
			finish = false;
		} else if (view->outputBuffer[0] == '\b') {
			moveFollowing(viewNumber, newCursorX, newCursorY, -1);
			newCursorX--;
			setCursorAt(viewNumber, newCursorX, newCursorY);
			// view->textBuffer[newCursorY][newCursorX] = '\0';
			redrawCharInverted(view, view->cursorX, view->cursorY);
			consume(view, 1);
			finish = false;
		} else if (view->outputBuffer[0] == '\r') {
			for (int x = 0; x < newCursorX; x++) {
				view->textBuffer[newCursorY][x] = '\0';
				redrawChar(view, x, newCursorY);
			}
			newCursorX = 0;
			setCursorAt(viewNumber, newCursorX, newCursorY);
			// view->textBuffer[newCursorY][newCursorX] = '\0';
			redrawCharInverted(view, view->cursorX, view->cursorY);
			consume(view, 1);
			finish = false;
		} else if (isPrintable(view->outputBuffer[0]) ||
		           view->outputBuffer[0] == '\0') {
			setChar(viewNumber, view->outputBuffer[0]);
			consume(view, 1);
			newCursorX++;
			finish = false;
		} else {
			consume(view, 1);
			finish = false;
		}
		setCursorAt(viewNumber, newCursorX, newCursorY);
		newCursorX = view->cursorX;
		newCursorY = view->cursorY;
	}
}

void setViewGraphic(uint8_t viewNumber, bool value) {
	struct View *view = &Views[viewNumber];
	view->graphic = value;
	reDraw(view);
}

void setCursorAt(uint8_t viewNumber, int x, int y) {
	struct View *view = &Views[viewNumber];

	if (x < 0 && y <= 0) {
		x = 0;
		y = 0;
	}
	while (x >= (int)view->width) {
		x = 0;
		y++;
	}
	while (x < 0) {
		x = view->width - 1;
		y--;
	}
	y = scrollTo(viewNumber, y);
	if (view->cursorX != x || view->cursorY != y) {
		redrawChar(view, view->cursorX, view->cursorY);
		redrawCharInverted(view, x, y);
	}
	view->cursorX = x;
	view->cursorY = y;
}
// uint32_t getCursorY() {
//     return cursorY;
// }
// uint32_t getCursorX() {
//     return cursorX;
// }

void print(uint8_t viewNumber, char *str) {
	while (*str) {
		printChar(viewNumber, *str++);
	}
}

void clear(uint8_t viewNumber) {
	struct View *view = &Views[viewNumber];

	view->scrollY = 0;
	for (int y = 0; y < TEXT_BUFFER_HEIGHT; y++) {
		for (int x = 0; x < TEXT_BUFFER_WIDTH; x++) {
			view->textBuffer[y][x] = '\0';
		}
	}
	view->cursorX = 0;
	view->cursorY = 0;
	reDraw(view);
}

// Helper print functions for Kernel space
void printIntN(uint8_t viewNumber, int value, uint8_t digits, uint8_t base) {
	char str[digits + 1];
	numToString(value, digits, str, base);
	print(viewNumber, str);
}
void printInt(uint8_t viewNumber, int value, uint8_t base) {
	printIntN(viewNumber, value, countDigits(value, base), base);
}

void printUnsignedN(uint8_t viewNumber, uint64_t value, uint8_t digits,
                    uint8_t base) {
	char str[digits + 1];
	unsignedToString(value, digits, str, base);
	print(viewNumber, str);
}

void printUnsigned(uint8_t viewNumber, uint64_t value, uint8_t base) {
	printUnsignedN(viewNumber, value, countDigits(value, base), base);
}

void printHexPrefix(uint8_t viewNumber) {
	printChar(viewNumber, '0');
	printChar(viewNumber, 'x');
}

void printHexByte(uint8_t viewNumber, uint8_t value) {
	printHexPrefix(viewNumber);
	printUnsignedN(viewNumber, value, 2, 16);
}

void printHexPointer(uint8_t viewNumber, void *ptr) {
	printHexPrefix(viewNumber);
	printUnsignedN(viewNumber, (uint64_t)ptr, 16, 16);
}

void drawCircle(uint8_t viewNumber, uint16_t x, uint16_t y, uint16_t radius) {
	struct View *view = &Views[viewNumber];

	drawCircleRaw(getOffsetX() + (view->positionX * getFontWidth()) + x,
	              getOffsetY() + (view->positionY * getFontHeight()) + y,
	              radius, view->colors.foreground, VIEWCLIP);
}

void drawRectangle(uint8_t viewNumber, uint16_t x, uint16_t y, uint16_t width,
                   uint16_t height) {
	struct View *view = &Views[viewNumber];

	drawRectangleRaw(getOffsetX() + (view->positionX * getFontWidth()) + x,
	                 getOffsetY() + (view->positionY * getFontHeight()) + y,
	                 width, height, view->colors.foreground, VIEWCLIP);
}

void drawBitmap(uint8_t viewNumber, uint16_t x, uint16_t y, uint16_t width,
                uint16_t height, Color bitmap[][width]) {
	struct View *view = &Views[viewNumber];
	drawBitmapRaw(getOffsetX() + (view->positionX * getFontWidth()) + x,
	              getOffsetY() + (view->positionY * getFontHeight()) + y, width,
	              height, bitmap, VIEWCLIP);
}

void getViewInfo(uint8_t viewNumber, WindowInfo *windowInfo) {
	struct View *view = &Views[viewNumber];

	windowInfo->pixelWidth = view->width * getFontWidth();
	windowInfo->pixelHeight = view->height * getFontHeight();
	windowInfo->textWidth = view->width;
	windowInfo->textHeight = view->height;
}