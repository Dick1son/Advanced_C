#include <curses.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define MIN_Y 2

enum { UP, LEFT, DOWN, RIGHT, STOP_GAME = KEY_F(10) };
enum {
	MAX_TAIL_SIZE = 100, START_TAIL_SIZE = 1, MAX_FOOD_SIZE = 20,
	FOOD_EXPIRE_SECONDS = 10
};

struct control_buttons
{
	int up;
	int left;
	int down;
	int right;
}control_buttons;
struct control_buttons default_controls[4] = {	{ 3,   4,   2,   5},
												{'w', 'a', 's', 'd'},
												{'F', 'D', 'K', '2'},
												{'&', '$', '+', 18} };

typedef struct snake_t
{
	int x;
	int y;
	int direction;
	size_t tsize;
	struct tail_t* tail;
	struct control_buttons controls[4];
} snake_t;

typedef struct tail_t
{
	int x;
	int y;
} tail_t;



void initTail(struct tail_t t[], size_t size)
{
	struct tail_t init_t = { 0,0 };
	for (size_t i = 0; i < size; i++)
	{
		t[i] = init_t;
	}
}
void initHead(struct snake_t* head, int x, int y)
{
	head->x = x;
	head->y = y;
	head->direction = RIGHT;
}
void initSnake(snake_t* head, size_t size, int x, int y)
{
	tail_t* tail = (tail_t*)
		malloc(MAX_TAIL_SIZE * sizeof(tail_t));
	initTail(tail, MAX_TAIL_SIZE);
	initHead(head, x, y);
	head->tail = tail;
	head->tsize = size + 1;
	for (uint8_t i = 0; i < sizeof(default_controls) / sizeof(default_controls[0]); i++)
		head->controls[i] = default_controls[i];
}

void go(struct snake_t* head, int max_x, int max_y) {
	init_pair(2, COLOR_GREEN, COLOR_BLUE);
	attrset(A_BOLD | COLOR_PAIR(2));

	char ch = '@';
	mvprintw(head->y, head->x, " ");
	switch (head->direction)
	{
	case LEFT:
		if (head->x <= 0)
			head->x = max_x;
		mvprintw(head->y, --(head->x), "%c", ch);
		break;
	case RIGHT:
		if (head->x >= max_x)
			head->x = 0;
		mvprintw(head->y, ++(head->x), "%c", ch);
		break;
	case UP:
		if (head->y <= MIN_Y)
			head->y = max_y;
		mvprintw(--(head->y), head->x, "%c", ch);
		break;
	case DOWN:
		if (head->y >= max_y)
			head->y = MIN_Y;
		mvprintw(++(head->y), head->x, "%c", ch);
		break;
	default:
		break;
	}
	refresh();
}

void goTail(struct snake_t* head)
{
	char ch = '#';
	mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");
	for (size_t i = head->tsize - 1; i > 0; i--)
	{
		head->tail[i] = head->tail[i - 1];
		if (head->tail[i].y || head->tail[i].x)
			mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
	}
	head->tail[0].x = head->x;
	head->tail[0].y = head->y;
}

int checkDirection(snake_t* snake, int32_t key) {
	int temp_direction = snake->direction;
	int res = 0;

	for (uint8_t i = 0; i < sizeof(default_controls) / sizeof(default_controls[0]); i++) {
		if (((temp_direction == UP || temp_direction == DOWN) && (key == snake->controls[i].down || key == snake->controls[i].up))
			|| ((temp_direction == LEFT || temp_direction == RIGHT) && (key == snake->controls[i].right || key == snake->controls[i].left))) {

			res--;
		}
		else {
			res++;
		}

	}
	return res;
}

void changeDirection(snake_t* snake, const int32_t key) {

	if (checkDirection(snake, key) == 4) {
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
}

int main() {
	snake_t* snake = (snake_t*)malloc(sizeof(snake_t));
	initSnake(snake, START_TAIL_SIZE, 10, 10);

	initscr();
	keypad(stdscr, TRUE);
	raw();
	noecho();
	curs_set(FALSE);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	bkgd(COLOR_PAIR(1));
	attrset(A_DIM | COLOR_PAIR(1));

	mvprintw(1, 0, "Use arrows or WASD for control. Press 'F10' for EXIT");
	timeout(0);

	int max_x = 0, max_y = 0;
	getmaxyx(stdscr, max_y, max_x);

	char key_pressed = 0;
	while (key_pressed != STOP_GAME) {
		key_pressed = tolower(getch());
		go(snake, max_x, max_y);
		goTail(snake);
		timeout(100);
		changeDirection(snake, key_pressed);
	}

	free(snake->tail);
	free(snake);
	endwin();
	return 0;
}