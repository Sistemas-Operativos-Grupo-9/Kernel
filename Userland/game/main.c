#include <color.h>
#include <print.h>
#include <random.h>
#include <stdio.h>
#include <syscall.h>

typedef struct {
	float x, y;
} Vector;

typedef struct {
	Vector pos;
	Vector speed;
	Color color;
} Marble;

#define RADIUS 20

typedef struct {
	Marble marbles[64];
	Vector gravity;
	int count;
} State;

WindowInfo windowInfo;

State loop(State state, float dt) {
	State newState = {.count = state.count, .gravity = state.gravity};
	Vector gravity = state.gravity;
	for (int i = 0; i < state.count; i++) {
		Vector pos = state.marbles[i].pos;
		Vector speed = state.marbles[i].speed;

		speed.x += gravity.x * dt;
		speed.y += gravity.y * dt;
		pos.x += speed.x * dt;
		pos.y += speed.y * dt;
		if ((pos.x < RADIUS && speed.x < 0) ||
		    (pos.x > windowInfo.pixelWidth - RADIUS && speed.x > 0)) {
			speed.x = -speed.x;
		}
		if ((pos.y < RADIUS && speed.y < 0) ||
		    (pos.y > windowInfo.pixelHeight - RADIUS && speed.y > 0)) {
			speed.y = -speed.y;
		}

		newState.marbles[i] = (Marble){
		    .pos = pos,
		    .speed = speed,
		    .color = state.marbles[i].color,
		};
	}

	return newState;
}

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

#define drawByPixel(xStart, yStart, xEnd, yEnd, code)                          \
	for (int y = yStart; y < yEnd; y++) {                                      \
		for (int x = xStart; x < xEnd; x++) {                                  \
			code                                                               \
		}                                                                      \
	}

void drawCircleLocal(Color buffer[][windowInfo.pixelWidth], uint64_t centerX,
                     uint64_t centerY, uint64_t radius, Color color) {
	drawByPixel(
	    max(0, centerX - radius), max(0, centerY - radius),
	    min(windowInfo.pixelWidth, centerX + radius),
	    min(windowInfo.pixelHeight, centerY + radius),
	    if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <
	        radius * radius) buffer[y][x] = color;)
}

void render(State state) {
	for (int i = 0; i < state.count; i++) {
		Color color = state.marbles[i].color;
		Vector pos = state.marbles[i].pos;
		setForeground(color);
		drawCircle((int)pos.x, (int)pos.y, RADIUS);
	}
	flip();
}
Vector randomVector() {
	Vector vec;
	vec.x = rand() % windowInfo.pixelWidth;
	vec.y = rand() % windowInfo.pixelHeight;
	return vec;
}

Color randomColor() {
	return (Color){
	    .red = rand() % 256, .green = rand() % 256, .blue = rand() % 256};
}
int main() {
	windowInfo = getWindowInfo();

	setForeground(BLACK);
	drawRectangle(0, 0, windowInfo.pixelWidth, windowInfo.pixelHeight);
	setGraphic(true);

	State state = {.count = 10, .gravity = {.x = 0, .y = 100}};
	srand(millis());

	for (int i = 0; i < state.count; i++) {
		state.marbles[i].pos = randomVector();
		Vector speed = randomVector();
		speed.x -= windowInfo.pixelWidth / 2;
		speed.y -= windowInfo.pixelHeight / 2;
		speed.x *= 1;
		state.marbles[i].speed = speed;
		state.marbles[i].color = randomColor();
	}

	int start = millis();
	int last = start;
	int current;
	while (((current = millis()) - start) / 2000 < 2) {
		state = loop(state, (float)(current - last) / 1000);
		render(state);
		last = current;
		millisleep(10);
	}

	setForeground(WHITE);
	setGraphic(false);
	return 0;
}