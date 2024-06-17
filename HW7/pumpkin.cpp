#include "pumpkin.h"

uint8_t num_active_pumpkin;

void _putPumpkin(struct pumpkin_t* pumpkin_ptr){
	attron(COLOR_PAIR(GREEN));

	pumpkin_ptr->maturation_time = MATURATION_TIME_MIN + rand() % (MATURATION_TIME_MAX - MATURATION_TIME_MIN + 1);
	
	pumpkin_ptr->y = (MAP_MIN_Y + 1) + rand() % ((MAP_MAX_Y - 1) - (MAP_MIN_Y + 1) + 1);
	pumpkin_ptr->x = (MAP_MIN_X + 30) + rand() % ((MAP_MAX_X - 1) - (MAP_MIN_X + 30) + 1);
	
	mvaddwstr(pumpkin_ptr->y, pumpkin_ptr->x, PUMPKIN_CH);
	pumpkin_ptr->put_time = pumpkin_time->tm_sec;
	attroff(COLOR_PAIR(GREEN));
}

void putPumpkin() {
	num_active_pumpkin = rand() % (PUMPKIN_MAX_QUANTITY - PUMPKIN_MIN_QUANTITY + 1) + PUMPKIN_MIN_QUANTITY;
	for (uint8_t i = 0; i < num_active_pumpkin; i++) {
		_putPumpkin(&harvest[i]);
	}
}

void _changeStatus(struct pumpkin_t* pumpkin_ptr) {
	attron(COLOR_PAIR(ORANGE));
	if (pumpkin_ptr->put_time + pumpkin_ptr->maturation_time <= pumpkin_time->tm_sec && pumpkin_ptr->status == 0) {
		mvaddwstr(pumpkin_ptr->y, pumpkin_ptr->x, PUMPKIN_CH);
		pumpkin_ptr->status = 1;
	}
		
	attroff(COLOR_PAIR(ORANGE));
}

void changeStatus() {
	for (uint8_t i = 0; i < num_active_pumpkin; i++) {
		_changeStatus(&harvest[i]);
	}
}

bool checkPickUp(drone_t* drone) {

	for (uint8_t i = 0; i < num_active_pumpkin; i++) {
		if (drone->y == harvest[i].y && drone->x == harvest[i].x && harvest[i].status == 1) {
			drone->level++;
			drone->trailer_size++;

			harvest[i] = { 0, 0, 0, 0, 0 };
			return 1;
		}
		else if (drone->y == harvest[i].y && drone->x == harvest[i].x && harvest[i].status == 0)
			return 0;
	}
	return 0;
}

void printLevel(drone_t* drone) {
	mvprintw(MAP_MIN_Y - 1, MAP_MIN_X + 17, "< %d >", drone->level);
}

//I tried...(
//void repairPumpkin(drone_t* drone) {
//	attron(COLOR_PAIR(GREEN));
//
//	for (uint8_t i = 0; i < num_repair_pumpkin; i++) {
//		if (drone->trailer_size - 1 == 0) {
//			if (repair[i].y != drone->y && repair[i].x != drone->x) {
//				mvaddwstr(repair[i].y, repair[i].x, PUMPKIN_CH);
//				num_repair_pumpkin--;
//			}
//		}
//		else {
//			for (uint8_t j = 0; j < drone->trailer_size; j++) {
//				if ((repair[i].y != drone->y && repair[i].x != drone->x) && (repair[i].y != drone->trailer[j].y && repair[i].x != drone->trailer[j].x)) {
//					mvaddwstr(repair[i].y, repair[i].x, PUMPKIN_CH);
//					num_repair_pumpkin--;
//				}
//			}
//		}
//	}
//
//	attroff(COLOR_PAIR(GREEN));
//}

