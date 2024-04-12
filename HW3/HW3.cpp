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
} snake_t;

typedef struct food_t {
	int x;
	int y;
	char point;
} food_t;

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
void initSnake(snake_t* head, size_t size, int x, int y, char head_char, char tail_char) {
	tail_t* tail = (tail_t*) malloc(MAX_TAIL_SIZE * sizeof(tail_t));
	initTail(tail, MAX_TAIL_SIZE);
	initHead(head, x, y);
	head->tail = tail;
	head->tail_size = size + 1;
	head->head_char = head_char;
	head->tail_char = tail_char;
	head->level = 0;
	for (uint8_t i = 0; i < sizeof(default_controls) / sizeof(default_controls[0]); i++)
		head->controls[i] = default_controls[i];
}
void initFood(food_t* food, int x, int y, char point) {
	food->x = x;
	food->y = y;
	food->point = point;
}

void go(struct snake_t* head) {
	attrset(A_BOLD | COLOR_PAIR(2));

	mvprintw(head->y, head->x, " ");
	switch (head->direction)
	{
	case LEFT:
		if (head->x <= MIN_X)
			head->x = MAX_X;
		mvprintw(head->y, --(head->x), "%c", head->head_char);
		break;
	case RIGHT:
		if (head->x >= MAX_X)
			head->x = MIN_X;
		mvprintw(head->y, ++(head->x), "%c", head->head_char);
		break;
	case UP:
		if (head->y <= MIN_Y)
			head->y = MAX_Y;
		mvprintw(--(head->y), head->x, "%c", head->head_char);
		break;
	case DOWN:
		if (head->y >= MAX_Y)
			head->y = MIN_Y;
		mvprintw(++(head->y), head->x, "%c", head->head_char);
		break;
	default:
		break;
	}
}

void goTail(struct snake_t* head) {
	mvprintw(head->tail[head->tail_size - 1].y, head->tail[head->tail_size - 1].x, " ");
	for (size_t i = head->tail_size - 1; i > 0; i--)
	{
		head->tail[i] = head->tail[i - 1];
		if (head->tail[i].y || head->tail[i].x)
			mvprintw(head->tail[i].y, head->tail[i].x, "%c", head->tail_char);
	}
	head->tail[0].x = head->x;
	head->tail[0].y = head->y;
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

bool checkEat(snake_t* snake, food_t* food) {
	if (snake->x == food->x && snake->y == food->y) {
		snake->level++;
		snake->tail_size++;
		return 1;
	}
	return 0;
}

bool checkWin(snake_t* snake) {
	if (snake->level == MAX_LEVEL) {
		attrset(A_BOLD | COLOR_PAIR(2));
		mvprintw(1, 52, "WIN!!!");
		return 1;
	}
	else {
		for (uint16_t i = 2; i < snake->tail_size - 1; i++) {
			if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y) {
				attrset(A_BOLD | COLOR_PAIR(3));
				mvprintw(1, 52, "LOSE ");
				return 1;
			}
		}
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

void putFood(food_t* food, snake_t* snake) {
	attrset(A_BOLD | COLOR_PAIR(3));
	srand(time(NULL));
	uint16_t i = 0;
	uint16_t put_verify = 0;
	food->x = rand() % ((MAX_X - 1) - (MIN_X + 1) + 1) + MIN_X + 1;
	food->y = rand() % ((MAX_Y - 1) - (MIN_Y + 1) + 1) + MIN_Y + 1;

	while (put_verify != snake->tail_size)
	{
		if ((food->x != snake->x || food->y != snake->y) && (food->x != snake->tail[i].x || food->y != snake->tail[i].y))
			put_verify++;
		if (i == snake->tail_size - 1) {
			i = 0;
			srand(time(NULL));
			food->x = rand() % ((MAX_X - 1) - (MIN_X + 1) + 1) + MIN_X + 1;
			food->y = rand() % ((MAX_Y - 1) - (MIN_Y + 1) + 1) + MIN_Y + 1;
		}
		
		i++;
	}
	mvprintw(food->y, food->x, "%c", food->point);
}

void showMap(snake_t* snake) {

	attrset(A_BOLD | COLOR_PAIR(1));

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

int main() {
	double DELAY = 0.15;
	char key_pressed = 0;
	snake_t* snake = (snake_t*) malloc(sizeof(snake_t));
	food_t* food = (food_t*)malloc(sizeof(food_t));
	bool pause_flag = 0;

	initSnake(snake, START_TAIL_SIZE, 12, 10, '@', '#');
	initFood(food, 0, 0, '+');
	initscr();

	keypad(stdscr, TRUE);
	raw();
	noecho();
	curs_set(FALSE);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_GREEN, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_BLUE);
	bkgd(COLOR_PAIR(1));
	attrset(A_BOLD | COLOR_PAIR(1));

	timeout(0);

	putFood(food, snake);
	while (key_pressed != EXIT_GAME) {
		key_pressed = tolower(getch());
		if (key_pressed == 55 || key_pressed == 23)
			key_pressed = 112;

		if (key_pressed != PAUSE && pause_flag == 0) {
			clock_t begin = clock();
			showMap(snake);
			go(snake);
			goTail(snake);

			if (pause_flag = checkWin(snake)) {
				showMap(snake);
				continue;
			}else if (checkEat(snake, food)) {
					if ((snake->level + 1) % (MAX_LEVEL / 10) == 0)
						DELAY -= 0.01;
					putFood(food, snake);
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
			attrset(A_BOLD | COLOR_PAIR(3));
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