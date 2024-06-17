#include "init.h"

//Global variables
int max_y, max_x;
struct pumpkin_t harvest[PUMPKIN_MAX_QUANTITY];
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
	init_pair(8, COLOR_ORANGE, COLOR_BLUE);

	//Activate fullscreen mode
	COORD coord;
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, &coord);

	//Set and resize font
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo;

	fontInfo.cbSize = sizeof(fontInfo);

	GetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);

	wcscpy(fontInfo.FaceName, L"Lucida Console");

	fontInfo.dwFontSize.Y = 30;
	SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);

	//Hide windows cursor
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);

	//Hide windows scrollbar
	ShowScrollBar(GetConsoleWindow(), SB_VERT, 0);

	//Resize curses terminal
	resize_term(2000, 2000);
	getmaxyx(stdscr, max_y, max_x);
	resize_term(max_y, max_x);

	bkgd(COLOR_PAIR(1));
}

void _initTrailer(trailer_t t[], size_t size) {
	trailer_t init_t = { 0,0 };
	for (size_t i = 0; i < size; i++)
	{
		t[i] = init_t;
	}
}

void initDrone(drone_t* drone, uint8_t color) {
	trailer_t* trailer = (trailer_t*)malloc(TRAILER_MAX_SIZE * sizeof(trailer_t));
	_initTrailer(trailer, TRAILER_MAX_SIZE);
	drone->x = 25;
	drone->y = 15;
	drone->direction = RIGHT;
	drone->trailer = trailer;
	drone->trailer_size = TRAILER_START_SIZE + 1;
	drone->level = 0;
	drone->color = color;
	drone->controls = default_controls;
}

void initPumpkin() {

	for (uint8_t i = 0; i < PUMPKIN_MAX_QUANTITY; i++) 
			harvest[i] = { 0, 0, 0, 0, 0 };
		
}

void initMap() {
	//Print interface 
	mvaddwstr(MAP_MIN_Y - 2, (MAP_MAX_X - MAP_MIN_X) / 4 + MAP_MIN_X, STR_HEADER);
	mvaddwstr(MAP_MIN_Y - 1, MAP_MIN_X , STR_LEVEL);
	mvaddwstr(MAP_MIN_Y - 1, MAP_MAX_X - 12, STR_TIME);

	//Print corners
	mvaddwstr(MAP_MIN_Y, MAP_MIN_X, TL);
	mvaddwstr(MAP_MIN_Y, MAP_MIN_X + 29, TT);
	mvaddwstr(MAP_MIN_Y, MAP_MAX_X, TR);

	mvaddwstr(MAP_MAX_Y, MAP_MIN_X, BL);
	mvaddwstr(MAP_MAX_Y, MAP_MIN_X + 29, BT);
	mvaddwstr(MAP_MAX_Y, MAP_MAX_X, BR);

	//Print borders
	for (uint16_t x = MAP_MIN_X + 1; x < MAP_MAX_X; x++) {
		if (x == MAP_MIN_X + 29)
			continue;
		mvaddwstr(MAP_MIN_Y, x, HS);
		mvaddwstr(MAP_MAX_Y, x, HS);
	}
	for (uint16_t y = MAP_MIN_Y + 1; y < MAP_MAX_Y; y++) {
		mvaddwstr(y, MAP_MIN_X, VS);
		mvaddwstr(y, MAP_MAX_X, VS);

		if(y <= MAP_MIN_Y + 8 || y >= MAP_MAX_Y - 8)
			mvaddwstr(y, MAP_MIN_X + 29, VS);
	}
}
