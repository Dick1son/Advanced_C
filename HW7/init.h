#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <curses.h>
#include <ctype.h>
#include <time.h>
#include "unicode_chars.h"

#define MAP_MIN_Y 3
#define MAP_MIN_X 5
#define MAP_MAX_Y max_y - MAP_MIN_Y
#define MAP_MAX_X max_x - MAP_MIN_X

#define PDC_WIDE
#define COLOR_ORANGE (COLOR_YELLOW | COLOR_GREEN) 

enum { UP, LEFT, DOWN, RIGHT, PLAY = 1, PAUSE, EXIT };
enum { MAX_NUM_WAVE = 5, TRAILER_MAX_SIZE = 15, TRAILER_START_SIZE = 0, PUMPKIN_MAX_QUANTITY = 15, PUMPKIN_MIN_QUANTITY = 5, MATURATION_TIME_MIN = 10, MATURATION_TIME_MAX = 18};
enum { BKGD = 1, GREEN, RED, CYAN, MAGENTA, YELLOW, BLACK, ORANGE };

struct control_buttons {
	int up;
	int left;
	int down;
	int right;
};

typedef struct trailer_t{
	uint16_t x;
	uint16_t y;
} trailer_t;

typedef struct drone_t {
	uint16_t x;
	uint16_t y;
	uint16_t level;
	size_t trailer_size;
	struct trailer_t* trailer;
	struct control_buttons controls;
	uint8_t direction;
	uint8_t color;
} drone_t;

struct pumpkin_t {
	time_t put_time;
	uint16_t x;
	uint16_t y;
	uint8_t maturation_time;
	bool status;
};

extern int max_y, max_x;
extern struct control_buttons default_controls;
extern struct pumpkin_t harvest[];

void initScreen  (void);
void initDrone   (drone_t*, uint8_t);
void initPumpkin (void);
void initMap     (void);