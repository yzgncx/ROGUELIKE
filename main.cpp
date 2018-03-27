#include <ncursesw/curses.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "Game.h"
#include "GameWindow.h"
#include "Menu.h"
#include "Stats.h"

int main(void)
{
        setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	start_color();
       	curs_set(0);

	int y, x;
	getmaxyx(stdscr, y, x);
	std::srand((unsigned int)time(NULL));

	Game g;
	g.newGame();

	AdventureWindow a(&g);
	a.runWindow();
	
	endwin();
        return 0;
}
