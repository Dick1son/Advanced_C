#include <curses.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>

#define MIN_Y 3
#define MAX_Y 26
#define MIN_X 10
#define MAX_X 100

enum { UP, LEFT, DOWN, RIGHT, EXIT_GAME = 32, PAUSE = 112 };
enum { MAX_LEVEL = 10, MAX_TAIL_SIZE = 1845, START_TAIL_SIZE = 1 };

struct control_buttons {
	int up;
	int left;
	int down;
	int right;
}control_buttons;
struct control_buttons default_controls[4] = {	{ 3,   4,   2,   5},
												{'w', 'a', 's', 'd'},
												{'F', 'D', 'K', '2'},
												{'&', '$', '+', 18} };

typedef struct tail_t {
	int x;
	int y;
} tail_t;

typedef struct snake_t {
	int x;
	int y;
	int direction;
	uint16_t level;
	size_t tail_size;
	struct tail_t* tail;
	struct control_buttons controls[4];
	char head_char;
	char tail_char;
	uint8_t color;
} snake_t;

typedef struct food_t {
	int x;
	int y;
	char point;
} food_t;

int cursor(int y, int start_x, int end_x, int shift) {
	int cursor = start_x;
	int result = 0;
	char key = 0; 

	while (key != 13) {

		key = tolower(getch());

		if (key == 5 || key == 'd' || key == '2' || key == 18) {
			mvdelch(y, cursor);
			cursor += shift;
		}
		if (key == 4 || key == 'a' || key == 'D' || key == '$') {
			mvdelch(y, cursor);
			cursor -= shift;
		}

		if (cursor > end_x) cursor = start_x;
		if (cursor < start_x) cursor = end_x;

		mvprintw(y, cursor, "^");
		if (key == 13)
			result = (cursor - start_x) / shift;
	}

	return result;
}

void initTail(struct tail_t t[], size_t size) {
	struct tail_t init_t = { 0,0 };
	for (size_t i = 0; i < size; i++)
	{
		t[i] = init_t;
	}
}
void initHead(struct snake_t* head, int x, int y) {
	head->x = x;
	head->y = y;
	head->direction = RIGHT;
}
void initSnake(snake_t* head, size_t size, int x, int y, char head_char, char tail_char, uint8_t color) {
	tail_t* tail = (tail_t*) malloc(MAX_TAIL_SIZE * sizeof(tail_t));
	initTail(tail, MAX_TAIL_SIZE);
	initHead(head, x, y);
	head->tail = tail;
	head->tail_size = size + 1;
	head->head_char = head_char;
	head->tail_char = tail_char;
	head->level = 0;
	head->color = color;
	for (uint8_t i = 0; i < sizeof(default_controls) / sizeof(default_controls[0]); i++)
		head->controls[i] = default_controls[i];
}
void initFood(food_t* food, int x, int y, char point) {
	food->x = x;
	food->y = y;
	food->point = point;
}

void go(snake_t* snake) {
	attron(COLOR_PAIR(snake->color));

	mvprintw(snake->y, snake->x, " ");
	switch (snake->direction)
	{
	case LEFT:
		if (snake->x <= MIN_X)
			snake->x = MAX_X;
		mvprintw(snake->y, --(snake->x), "%c", snake->head_char);
		break;
	case RIGHT:
		if (snake->x >= MAX_X)
			snake->x = MIN_X;
		mvprintw(snake->y, ++(snake->x), "%c", snake->head_char);
		break;
	case UP:
		if (snake->y <= MIN_Y)
			snake->y = MAX_Y;
		mvprintw(--(snake->y), snake->x, "%c", snake->head_char);
		break;
	case DOWN:
		if (snake->y >= MAX_Y)
			snake->y = MIN_Y;
		mvprintw(++(snake->y), snake->x, "%c", snake->head_char);
		break;
	default:
		break;
	}
	attroff(COLOR_PAIR(snake->color));
}

void goTail(snake_t* snake) {
	attron(COLOR_PAIR(snake->color));
	mvprintw(snake->tail[snake->tail_size - 1].y, snake->tail[snake->tail_size - 1].x, " ");
	for (size_t i = snake->tail_size - 1; i > 0; i--)
	{
		snake->tail[i] = snake->tail[i - 1];
		if (snake->tail[i].y || snake->tail[i].x)
			mvprintw(snake->tail[i].y, snake->tail[i].x, "%c", snake->tail_char);
	}
	snake->tail[0].x = snake->x;
	snake->tail[0].y = snake->y;
	attroff(COLOR_PAIR(snake->color));
}

uint8_t checkDirection(snake_t* snake, int32_t key) {
	int temp_direction = snake->direction;
	uint8_t res = 0;

	for (uint8_t i = 0; i < sizeof(default_controls) / sizeof(default_controls[0]); i++) {
		if (((temp_direction == UP || temp_direction == DOWN) && (key == snake->controls[i].down || key == snake->controls[i].up))
			|| ((temp_direction == LEFT || temp_direction == RIGHT) && (key == snake->controls[i].right || key == snake->controls[i].left)) 
			|| (snake->x == MIN_X || snake->x == MAX_X || snake->y == MIN_Y || snake->y == MAX_Y)) {

			res--;
		}
		else {
			res++;
		}

	}
	return res;
}

bool checkEat(snake_t* snake, food_t* food, snake_t* snake_bot) {
	if (snake->x == food->x && snake->y == food->y) {
		snake->level++;
		snake->tail_size++;
		return 1;
	}
	if (snake_bot->x == food->x && snake_bot->y == food->y) {
		snake_bot->tail_size++;
		return 1;
	}
	return 0;
}

bool checkWin(snake_t* snake, snake_t* snake_bot) {
	if (snake->level == MAX_LEVEL) {
		attron(COLOR_PAIR(2));
		mvprintw(1, 52, "WIN!!!");
		return 1;
	}

	if (snake_bot->level == MAX_LEVEL) {
		attron(COLOR_PAIR(2));
		mvprintw(1, 52, "LOSE ");
		return 1;
	}
	for (uint16_t i = 1; i < snake->tail_size - 1; i++) {
		if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y){
			attron(COLOR_PAIR(3));
			mvprintw(1, 52, "LOSE ");
			return 1;
		}
	}
	for (uint16_t i = 0; i < snake_bot->tail_size - 1; i++) {
		if (snake->x == snake_bot->tail[i].x && snake->y == snake_bot->tail[i].y) {
			attron(COLOR_PAIR(3));
			mvprintw(1, 52, "LOSE ");
			return 1;
		}
	}
	if (snake->x == snake_bot->x && snake->y == snake_bot->y) {
		attron(COLOR_PAIR(3));
		mvprintw(1, 52, "LOSE ");
		return 1;
	}

	return 0;
}

void changeDirection(snake_t* snake, const int32_t key) {
	for (uint8_t i = 0; i < sizeof(default_controls) / sizeof(default_controls[0]); i++) {
		if (key == snake->controls[i].down)
			snake->direction = DOWN;
		else if (key == snake->controls[i].up)
			snake->direction = UP;
		else if (key == snake->controls[i].right)
			snake->direction = RIGHT;
		else if (key == snake->controls[i].left)
			snake->direction = LEFT;
	}
}

void putFood(food_t* food, snake_t* snake, snake_t* snake_bot) {
	attron(COLOR_PAIR(3));

	uint16_t i = 0;
	uint16_t j = 0;
	//uint16_t put_verify = 0;
	char put_place;
	bool put_verify = 0;
	food->x = rand() % ((MAX_X - 1) - (MIN_X + 1) + 1) + MIN_X + 1;
	food->y = rand() % ((MAX_Y - 1) - (MIN_Y + 1) + 1) + MIN_Y + 1;

	put_place = mvinch(food->y, food->x);
	//while (put_verify != snake->tail_size + snake_bot->tail_size)
	//{
	//	if ((food->x != snake->x || food->y != snake->y) && (food->x != snake->tail[i].x || food->y != snake->tail[i].y))
	//		put_verify++;
	//	if (i == snake->tail_size + snake_bot->tail_size - 1) {
	//		i = 0;
	//		srand(time(NULL));
	//		food->x = rand() % ((MAX_X - 1) - (MIN_X + 1) + 1) + MIN_X + 1;
	//		food->y = rand() % ((MAX_Y - 1) - (MIN_Y + 1) + 1) + MIN_Y + 1;
	//	}
	//	
	//	i++;
	//}
	while (put_verify == 0) {
		if (put_place != '#' || put_place != '@') {
			put_verify = 1;
			mvprintw(food->y, food->x, "%c", food->point);
		}
		else {
			srand(time(NULL));
			food->x = rand() % ((MAX_X - 1) - (MIN_X + 1) + 1) + MIN_X + 1;
			food->y = rand() % ((MAX_Y - 1) - (MIN_Y + 1) + 1) + MIN_Y + 1;
		}
	}
}

void showMap(snake_t* snake) {

	attron(COLOR_PAIR(1));

	mvprintw(1, MIN_X, "Level < %d > ", snake->level);

	mvprintw(1, MAX_X - 34, "\"p\" for pause, again \"p\" for start");
	mvprintw(2, MAX_X - 14, "Space for exit");

	for (uint8_t x = MIN_X; x <= MAX_X; x++) {
		for (uint8_t y = MIN_Y; y <= MAX_Y; y++) {
			if (y == MIN_Y || y == MAX_Y)
				mvprintw(y, x, "=");
			if (x == MIN_X || x == MAX_X)
				mvprintw(y, x, "|");
		}

	}
	
}

void generateSnakeDirection(snake_t* snake, food_t* food) {
	if ((snake->direction == LEFT || snake->direction == RIGHT) && snake->y == food->y) {
		return;
	}
	if ((snake->direction == LEFT || snake->direction == RIGHT) && snake->y != food->y) {
		if (food->x == snake->x && food->y < snake->y) {
			snake->direction = UP;
		}
		if (food->x == snake->x && food->y > snake->y) {
			snake->direction = DOWN;
		}
		return;
	}
	if ((snake->direction == UP || snake->direction == DOWN) && snake->x == food->x) {
		return;
	}
	if ((snake->direction == UP || snake->direction == DOWN) && snake->x != food->x) {
		if (food->y == snake->y && food->x > snake->x) {
			snake->direction = RIGHT;
		}
		if (food->y == snake->y && food->x < snake->x) {
			snake->direction = LEFT;
		}
		return;
	}
	if (snake->direction == UP && (mvinch(snake->y - 1, snake->x) == '#') || (mvinch(snake->y - 1, snake->x) == '@'))
		snake->direction = LEFT;
	if (snake->direction == DOWN && (mvinch(snake->y + 1, snake->x) == '#') || (mvinch(snake->y + 1, snake->x) == '@'))
		snake->direction = RIGHT;
	if (snake->direction == LEFT && (mvinch(snake->y, snake->x - 1) == '#') || (mvinch(snake->y, snake->x - 1) == '@'))
		snake->direction = UP;
	if (snake->direction == RIGHT && (mvinch(snake->y, snake->x - 1) == '#') || (mvinch(snake->y, snake->x - 1) == '@'))
		snake->direction = DOWN;
}

int8_t startMenu(snake_t* snake) {
	char key = 0;
	uint8_t color;
	uint8_t result;
	snake_t* snake_bot = 0;

	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_GREEN, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_BLUE);
	init_pair(4, COLOR_CYAN, COLOR_BLUE);
	init_pair(5, COLOR_MAGENTA, COLOR_BLUE);
	init_pair(6, COLOR_YELLOW, COLOR_BLUE);
	init_pair(7, COLOR_BLACK, COLOR_BLUE);

	noecho();
	curs_set(FALSE);

	mvprintw(3, 3, "choose mode:");
	mvprintw(3, 17, "Solo\tPvE");
	result = cursor(4, 18, 26, 7);

	clear();

	mvprintw(3, 3, "choose color:");

	for (int i = 2; i <= 7; i++) {
		attron(COLOR_PAIR(i));
		mvprintw(3, (i - 1) * 4 + 14, "@##");
		attroff(COLOR_PAIR(i));
	}

	color = cursor(4, 19, 39, 4) + 2;
	initSnake(snake, START_TAIL_SIZE, 12, 10, '@', '#', color);

	clear();
	return result;
}

int main() {
	double DELAY = 0.15;
	char key_pressed = 0;
	snake_t* snake = (snake_t*) malloc(sizeof(snake_t));
	snake_t* snake_bot = (snake_t*)malloc(sizeof(snake_t));
	food_t* food = (food_t*)malloc(sizeof(food_t));
	uint8_t bot_color;
	uint8_t mode;
	bool pause_flag = 0;

	initscr();

	keypad(stdscr, TRUE);
	raw();
	start_color();
	bkgd(COLOR_PAIR(1));
	srand(time(NULL));

	initSnake(snake_bot, START_TAIL_SIZE, 12, 12, '@', '#', 1);
	initFood(food, 0, 0, '$');

	timeout(0);

	mode = startMenu(snake);

	putFood(food, snake, snake_bot);
	while (key_pressed != EXIT_GAME) {
		key_pressed = tolower(getch());
		if (key_pressed == 55 || key_pressed == 23)
			key_pressed = 112;

		if (key_pressed != PAUSE && pause_flag == 0) {
			clock_t begin = clock();
			showMap(snake);

			if (mode) {
				generateSnakeDirection(snake_bot, food);
				go(snake);
				go(snake_bot);
				goTail(snake);
				goTail(snake_bot);
			}
			else {
				go(snake);
				goTail(snake);				
			}

			if (pause_flag = checkWin(snake, snake_bot)) {
				showMap(snake);
				continue;
			}
			else if (checkEat(snake, food, snake_bot)) {
					if ((snake->level + 1) % (MAX_LEVEL / 10) == 0)
						DELAY -= 0.01;
					putFood(food, snake, snake_bot);
			}

			if (checkDirection(snake, key_pressed) == sizeof(default_controls) / sizeof(default_controls[0]))
				changeDirection(snake, key_pressed);
			while ((double)(clock() - begin) / CLOCKS_PER_SEC < DELAY){

			}
			
		}
		else if (key_pressed == PAUSE && pause_flag == 1) {
			pause_flag = 0; 
			mvprintw(1, 52, "     ");
		}
		else if (key_pressed == PAUSE && pause_flag == 0) {
			attron(COLOR_PAIR(3));
			pause_flag = 1;
			mvprintw(1, 52, "PAUSE");
		}
	}

	free(snake->tail);
	free(snake);
	free(food);
	endwin();
	return 0;
}