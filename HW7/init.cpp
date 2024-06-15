#include "init.h"

int max_y, max_x;

struct control_buttons default_controls = { KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT };

void initScreen() {

	initscr();
	keypad(stdscr, TRUE);
	curs_set(0);
	raw();
	noecho();

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_GREEN, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_BLUE);
	init_pair(4, COLOR_CYAN, COLOR_BLUE);
	init_pair(5, COLOR_MAGENTA, COLOR_BLUE);
	init_pair(6, COLOR_YELLOW, COLOR_BLUE);
	init_pair(7, COLOR_BLACK, COLOR_BLUE);

	COORD coord;
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, &coord);


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo;

	fontInfo.cbSize = sizeof(fontInfo);

	GetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);

	wcscpy(fontInfo.FaceName, L"Lucida Console");

	fontInfo.dwFontSize.Y = 30;
	SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);

	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);

	resize_term(2000, 2000);
	getmaxyx(stdscr, max_y, max_x);
	resize_term(max_y, max_x);
	resize_term(max_y, max_x);

	bkgd(COLOR_PAIR(1));
}

void initTrailer(trailer_t t[], size_t size) {
	trailer_t init_t = { 0,0 };
	for (size_t i = 0; i < size; i++)
	{
		t[i] = init_t;
	}
}

void initDrone(drone_t* drone, uint8_t color) {
	trailer_t* trailer = (trailer_t*)malloc(MAX_TRAILER_SIZE * sizeof(trailer_t));
	initTrailer(trailer, MAX_TRAILER_SIZE);
	drone->x = 25;
	drone->y = 15;
	drone->direction = RIGHT;
	drone->trailer = trailer;
	drone->trailer_size = START_TRAILER_SIZE + 1;
	drone->level = 0;
	drone->color = color;
	drone->controls = default_controls;
}

void initPumpkin(pumpkin_t* pumpkin, int y, int x) {
	pumpkin->x = x;
	pumpkin->y = y;
}

void initMap() {
	mvaddwstr(MAP_MIN_Y, MAP_MIN_X, L"\x2554");
	mvaddwstr(MAP_MIN_Y, MAP_MAX_X, L"\x2557");
	mvaddwstr(MAP_MAX_Y, MAP_MIN_X, L"\x255a");
	mvaddwstr(MAP_MAX_Y, MAP_MAX_X, L"\x255d");
	for (uint16_t x = MAP_MIN_X + 1; x < MAP_MAX_X; x++) {
		mvaddwstr(MAP_MIN_Y, x, L"\x2550");
		mvaddwstr(MAP_MAX_Y, x, L"\x2550");
	}
	for (uint16_t y = MAP_MIN_Y + 1; y < MAP_MAX_Y; y++) {
		mvaddwstr(y, MAP_MIN_X, L"\x2551");
		mvaddwstr(y, MAP_MAX_X, L"\x2551");
	}
}