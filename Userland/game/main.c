#include <color.h>
#include <print.h>
#include <random.h>
#include <stdio.h>
#include <syscall.h>

typedef struct {
	int x, y;
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

State loop(State state, uint16_t dt) {
	State newState = {.count = state.count, .gravity = state.gravity};
	Vector gravity = state.gravity;
	for (int i = 0; i < state.count; i++) {
		Vector pos = state.marbles[i].pos;
		Vector speed = state.marbles[i].speed;

		speed.x += gravity.x * dt / 1000;
		speed.y += gravity.y * dt / 1000;
		pos.x += speed.x * dt / 1000;
		pos.y += speed.y * dt / 1000;
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

void render(State state) {
	setForeground(0, 0, 0);
	drawRectangle(0, 0, windowInfo.pixelWidth, windowInfo.pixelHeight);
	for (int i = 0; i < state.count; i++) {
		Color color = state.marbles[i].color;
		setForeground(color.red, color.green, color.blue);
		Vector pos = state.marbles[i].pos;
		drawCircle(pos.x, pos.y, RADIUS);
	}
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
	setGraphic(true);
	windowInfo = getWindowInfo();
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
		// printUnsigned(state.count, 10);
		state = loop(state, current - last);
		render(state);
		last = current;
		millisleep(13);
	}

	setForeground(255, 255, 255);
	setGraphic(false);
	return 0;
}