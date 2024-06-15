#include "move.h"


int main(){
	double DELAY = 0.1;
	int key_pressed = 0;
	int y = 0, x = 0;
	drone_t* drone = (drone_t*)malloc(sizeof(drone_t));
	pumpkin_t* pumpkin = (pumpkin_t*)malloc(sizeof(pumpkin_t));

	initScreen();
	initMap();
	initDrone(drone, 3);
	//initPumpkin(pumpkin, 12, 50); L"\x00f3"
	timeout(0);
	while (1) {
		clock_t begin = clock();
		key_pressed = tolower(getch());
		
		moveDrone(drone);

		if (checkDirection(drone, key_pressed))
			changeDirection(drone, key_pressed);

		timeout(200);
		refresh();
	}
	//mvaddwstr(2, 2, L"\x2551");
	//refresh();
	getchar();
	endwin();
	return 0;
}
