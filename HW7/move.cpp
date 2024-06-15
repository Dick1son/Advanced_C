#include "move.h"

void moveDrone(drone_t* drone) {
	attron(COLOR_PAIR(drone->color));


	//if (drone->direction != STOP)
	//	mvprintw(drone->y, drone->x, " ");

	switch (drone->direction)
	{
		case UP:
			if (drone->y == MAP_MIN_Y + 1)
				mvaddwstr(drone->y, drone->x, L"\x25b2");
			else {
				mvprintw(drone->y, drone->x, " ");
				mvaddwstr(--(drone->y), drone->x, L"\x25b2");
			}
			break;

		case DOWN:
			if (drone->y == MAP_MAX_Y - 1)
				mvaddwstr(drone->y, drone->x, L"\x25bc");
			else {
				mvprintw(drone->y, drone->x, " ");
				mvaddwstr(++(drone->y), drone->x, L"\x25bc");
			}
			break;

		case LEFT:
			if (drone->x == MAP_MIN_X + 1)
				mvaddwstr(drone->y, drone->x, L"\x25c4");
			else {
				mvprintw(drone->y, drone->x, " ");
				mvaddwstr(drone->y, --(drone->x), L"\x25c4");
			}
			break;

		case RIGHT:
			if (drone->x == MAP_MAX_X - 1)
				mvaddwstr(drone->y, drone->x, L"\x25ba");
			else {
				mvprintw(drone->y, drone->x, " ");
				mvaddwstr(drone->y, ++(drone->x), L"\x25ba");
			}
			break;

		default:
			break;
	}

	for (size_t i = drone->trailer_size - 1; i > 0; i--)
	{
		drone->trailer[i] = drone->trailer[i - 1];
		if (drone->trailer[i].y || drone->trailer[i].x)
			mvaddwstr(drone->trailer[i].y, drone->trailer[i].x, L"\x25cb");
	}
	drone->trailer[0].x = drone->x;
	drone->trailer[0].y = drone->y;

	attroff(COLOR_PAIR(drone->color));
}

//void moveTrailer(drone_t* drone) {
	//attron(COLOR_PAIR(drone->color));
	//mvprintw(drone->trailer[drone->trailer_size - 1].y, drone->trailer[drone->trailer_size - 1].x, " ");
//	for (size_t i = drone->trailer_size - 1; i > 0; i--)
//	{
//		drone->trailer[i] = drone->trailer[i - 1];
//		if (drone->trailer[i].y || drone->trailer[i].x)
//			mvwaddwstr(map,drone->trailer[i].y, drone->trailer[i].x, L"\x25cb");
//	}
//	drone->trailer[0].x = drone->x;
//	drone->trailer[0].y = drone->y;
//	attroff(COLOR_PAIR(drone->color));
//}

bool checkDirection(drone_t* drone, int32_t key) {
	if (((drone->direction == UP || drone->direction == DOWN) && (key == drone->controls.down || key == drone->controls.up))
		|| ((drone->direction == LEFT || drone->direction == RIGHT) && (key == drone->controls.right || key == drone->controls.left))
		|| (drone->x == MAP_MIN_X || drone->x == MAP_MAX_X || drone->y == MAP_MIN_Y || drone->y == MAP_MAX_Y)) {

		return 0;
	}
	
	return 1;
	
}

void changeDirection(drone_t* drone, const int32_t key) {
	if (key == drone->controls.down)
		drone->direction = DOWN;
	else if (key == drone->controls.up)
		drone->direction = UP;
	else if (key == drone->controls.right)
		drone->direction = RIGHT;
	else if (key == drone->controls.left)
		drone->direction = LEFT;
	//else
	//	drone->direction = STOP;
}