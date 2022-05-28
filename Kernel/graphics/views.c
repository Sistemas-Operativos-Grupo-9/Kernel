
#include "graphics/views.h"
#include "graphics/video.h"
#include "process.h"

static struct View {
	char inputBuffer[128];
	uint64_t bufferCount;
	bool eof;
} Views[10];

uint64_t inputAvailable(uint8_t tty) { return Views[tty].bufferCount; }

void writeChar(uint8_t tty, char ch) {
	Views[tty].inputBuffer[Views[tty].bufferCount++] = ch;
	keypressUpdate();
}

bool hasEof(uint8_t tty) {
	return Views[tty].eof;
}

void setEof(uint8_t tty, bool value) {
	Views[tty].eof = value;
}

char readInput(uint8_t tty) {
	struct View *view = &Views[tty];
	char ret = *view->inputBuffer;
	for (int i = 0; i < view->bufferCount; i++) {
		view->inputBuffer[i] = view->inputBuffer[i + 1];
	}
	view->bufferCount--;
	return ret;
}

void writeOutput(uint8_t tty, char ch) { ttyPutchar(tty, ch); }
