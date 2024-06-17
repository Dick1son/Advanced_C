#include "my_time.h"

time_t start_time;
time_t main_time;
struct tm* pumpkin_time;

void printTime() {
	int play_time = difftime(start_time, main_time) * -1;
	int minute = play_time / 60;
	int second = play_time % 60;

	if (second < 10)
		mvprintw(MAP_MIN_Y - 1, MAP_MAX_X - 5, "0%d:0%d", minute, second);
	else if (second >= 10)
		mvprintw(MAP_MIN_Y - 1, MAP_MAX_X - 5, "0%d:%d", minute, second);
	else if (minute >= 10 && second < 10)
		mvprintw(MAP_MIN_Y - 1, MAP_MAX_X - 5, "%d:0%d", minute, second);
	else
		mvprintw(MAP_MIN_Y - 1, MAP_MAX_X - 5, "%d:%d", minute, second);
}