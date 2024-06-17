#include "move.h"

void moveDrone(drone_t* drone) {
	attron(COLOR_PAIR(drone->color));

	//Move drone
	switch (drone->direction)
	{
		case UP:
			if (drone->y == MAP_MIN_Y + 1 || (drone->x == MAP_MIN_X + 29 && (drone->y == MAP_MIN_Y + 9 || drone->y == MAP_MAX_Y - 9)))
				mvaddwstr(drone->y, drone->x, DRONE_UP);
			else {
				mvprintw(drone->y, drone->x, " ");
				mvaddwstr(--(drone->y), drone->x, DRONE_UP);
			}
			break;

		case DOWN:
			if (drone->y == MAP_MAX_Y - 1 || (drone->x == MAP_MIN_X + 29 && (drone->y == MAP_MIN_Y + 9 || drone->y == MAP_MAX_Y - 9)))
				mvaddwstr(drone->y, drone->x, DRONE_DOWN);
			else {
				mvprintw(drone->y, drone->x, " ");
				mvaddwstr(++(drone->y), drone->x, DRONE_DOWN);
			}
			break;

		case LEFT:
			if (drone->x == MAP_MIN_X + 1 || (drone->x == MAP_MIN_X + 30 && (drone->y <= MAP_MIN_Y + 8 || drone->y >= MAP_MAX_Y - 8)))
				mvaddwstr(drone->y, drone->x, DRONE_LEFT);
			else {
				mvprintw(drone->y, drone->x, " ");
				mvaddwstr(drone->y, --(drone->x), DRONE_LEFT);
			}
			break;

		case RIGHT:
			if (drone->x == MAP_MAX_X - 1 || (drone->x == MAP_MIN_X + 28 && (drone->y <= MAP_MIN_Y + 8 || drone->y >= MAP_MAX_Y - 8)))
				mvaddwstr(drone->y, drone->x, DRONE_RIGHT);
			else {
				mvprintw(drone->y, drone->x, " ");
				mvaddwstr(drone->y, ++(drone->x), DRONE_RIGHT);
			}
			break;

		default:
			break;
	}

	//Move trailer
	mvprintw(drone->trailer[drone->trailer_size - 1].y, drone->trailer[drone->trailer_size - 1].x, " ");
	for (size_t i = drone->trailer_size - 1; i > 0; i--)
	{
		drone->trailer[i] = drone->trailer[i - 1];
		if (drone->trailer[i].y || drone->trailer[i].x)
			mvaddwstr(drone->trailer[i].y, drone->trailer[i].x, TRAILER_CH);
	}
	drone->trailer[0].x = drone->x;
	drone->trailer[0].y = drone->y;

	attroff(COLOR_PAIR(drone->color));
}

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
}