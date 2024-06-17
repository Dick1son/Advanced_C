#include "move.h"
#include "pumpkin.h"
#include "my_time.h"
#include "logic.h"

int main() {
	const char* project = "drone";
	const char* sources[] = { "main", "init", "logic", "move", "my_time", "pumpkin"};
	const char* headers[] = { "unicode_chars","init", "logic", "move", "my_time", "pumpkin", "curses"};

	double DELAY = 0.15;
	int key_pressed = 0;

	initScreen();
	initMap();

	time(&start_time);
	time(&main_time);
	pumpkin_time = localtime(&start_time);

	srand(time(NULL));

	drone_t* drone = (drone_t*)malloc(sizeof(drone_t));

	initPumpkin();

	initDrone(drone, RED);

	putPumpkin();

	timeout(0);

	while (key_pressed != 'q') {
		printTime();
		printLevel(drone);
		if (pumpkin_time->tm_sec >= 60 - MATURATION_TIME_MAX) {
			pumpkin_time->tm_sec = 0;
			for (uint8_t i = 0; i < PUMPKIN_MAX_QUANTITY; i++) {
				harvest[i].put_time = 0;
			}
		}
		clock_t begin = clock();
		key_pressed = tolower(getch());

		moveDrone(drone);

		checkPickUp(drone);

		changeStatus();
		if (checkDirection(drone, key_pressed))
			changeDirection(drone, key_pressed);

		refresh();
		
		while ((double)(clock() - begin) / CLOCKS_PER_SEC < DELAY) {
			time(&main_time);
			pumpkin_time = localtime(&main_time);
		}

	}

	free(drone);
	for (uint8_t i = 0; i < PUMPKIN_MAX_QUANTITY; i++) {
		free(&harvest[i]);
	}
	free(pumpkin_time);
	endwin();
	return 0;
}
