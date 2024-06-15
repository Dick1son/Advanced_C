#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <curses.h>
#include <ctype.h>
#include <time.h>

#define MAP_MIN_Y 3
#define MAP_MIN_X 5
#define MAP_MAX_Y max_y - MAP_MIN_Y
#define MAP_MAX_X max_x - MAP_MIN_X

#define PDC_WIDE

extern int max_y, max_x;

enum { STOP, UP, LEFT, DOWN, RIGHT, EXIT_GAME = 32, PAUSE = 112 };
enum { MAX_LEVEL = 10, MAX_TRAILER_SIZE = 1845, START_TRAILER_SIZE = 0 };

struct control_buttons {
	int up;
	int left;
	int down;
	int right;
};

extern struct control_buttons default_controls;

typedef struct trailer_t{
	int x;
	int y;
} trailer_t;

typedef struct drone_t {
	int x;
	int y;
	int direction;
	uint16_t level;
	size_t trailer_size;
	struct trailer_t* trailer;
	struct control_buttons controls;
	uint8_t color;
} drone_t;

typedef struct pumpkin_t {
	int x;
	int y;
} pumpkin_t;

void initScreen  (void);
void initDrone   (drone_t*, uint8_t);
void initPumpkin (pumpkin_t*, int, int);
void initMap     (void);