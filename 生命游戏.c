#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>
#define LENGTH 125
#define WIDTH  30
#define TIME 100000
#define RATE 3
#define LIVE "❥"

void creatRandom(_Bool [][LENGTH]);
void judge(_Bool space[][LENGTH]);
void printSpace(_Bool space[][LENGTH], WINDOW* windowPtr);
WINDOW* printWin();
int main()
{
	setlocale(LC_ALL,"");
	initscr();
	curs_set(0);

	_Bool space[WIDTH][LENGTH];
	WINDOW* windowPtr = printWin();
	if (windowPtr != NULL) {
		creatRandom(space);
		while(1) {
			printSpace(space, windowPtr);
			usleep(TIME);
			judge(space);
		}
	} else {
		endwin();
		puts("终端过小，无法显示窗口。");
		exit(1);
	}
	endwin ();
	exit (EXIT_SUCCESS);
	return 0;
}
void creatRandom(_Bool space[][LENGTH])
{
	srand ((unsigned) time (NULL));
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < LENGTH; j++) {
			if (rand () % RATE)
				space[i][j] = 0;
			else
				space[i][j] = 1;
	}
}
WINDOW* printWin()
{
	WINDOW *windowPtr = newwin (WIDTH + 2, LENGTH + 2, (LINES - WIDTH)/2, (COLS - LENGTH)/2);
	box(windowPtr, ACS_VLINE, ACS_HLINE);
	return windowPtr;
}
void judge(_Bool space[][LENGTH])
{
	_Bool copy[WIDTH][LENGTH];
	int adjacent[2][3];
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < LENGTH; j++)
			copy[i][j] = space[i][j];
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < LENGTH; j++) {
			int count = 0;
			adjacent[0][1] = i;
			adjacent[1][1] = j;
			if (i > 0 && i < WIDTH - 1) {
				adjacent[0][0] = i - 1;
				adjacent[0][2] = i + 1;
			} else if(i == 0) {
				adjacent[0][0] = WIDTH - 1;
				adjacent[0][2] = i + 1;
			}else if (i == WIDTH - 1) {
				adjacent[0][0] = i - 1;
				adjacent[0][2] = 0;
			}
			if (j > 0 && j < LENGTH - 1) {
				adjacent[1][0] = j - 1;
				adjacent[1][2] = j + 1;
			} else if(j == 0) {
				adjacent[1][0] = LENGTH - 1;
				adjacent[1][2] = j + 1;
			}else if (j == LENGTH - 1) {
				adjacent[1][0] = j - 1;
				adjacent[1][2] = 0;
			}
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++) {
					if (i == 1 && j == 1)
						continue;
					int y = adjacent[0][j];
					int x = adjacent[1][i];
					if (copy[y][x])
						count++;
				}
			if (space[i][j]) {
				if (count < 2 || count > 3)
					space[i][j] = 0;
			} else if (count == 3)
				space[i][j] = 1;
		}
}
void printSpace(_Bool space[][LENGTH], WINDOW* windowPtr)
{
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < LENGTH; j++) {
			if (space[i][j])
				mvwprintw(windowPtr, i + 1, j + 1, "%s", LIVE);
			else
				mvwprintw(windowPtr, i + 1, j + 1, "%c", ' ');
		}
	wrefresh(windowPtr);
}
