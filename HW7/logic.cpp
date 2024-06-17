#include "logic.h"

uint8_t game_status = PLAY;
uint8_t temp_wave = 0;

void printWin() {

}

void printLose() {

}

void printWave() {

}

void checkWin(drone_t* drone) {
	for (uint8_t i = 0; i < drone->trailer_size; i++) {
		if (drone->y == drone->trailer[i].y && drone->x == drone->trailer[i].x) {
			game_status = PAUSE;

		}

	}

	for (uint8_t i = 0; i < num_active_pumpkin; i++) {
		if (drone->y == harvest[i].y && drone->x == harvest[i].x && harvest[i].status == 0) {
			game_status = PAUSE;
		}

	}

	if (temp_wave == MAX_NUM_WAVE) {
		game_status = PAUSE;
		//win
	}
}