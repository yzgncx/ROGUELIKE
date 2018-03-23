#include  <ncurses.h>
#include "Game.h"
#include "GameWindow.h"

#include "Menu.h"
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Stats.h"

int main()
{
	
	int y, x;
	initscr();
	start_color();
	curs_set(0);
	getmaxyx(stdscr, y, x);
	std::srand((unsigned int)time(NULL));

	Game g;
	g.newGame();

	AdventureWindow a(&g);
	a.runWindow();

	/*
	std::vector<std::string> setA;
	std::vector<std::string> setB;
	std::vector<int> result;
	setA.push_back("16"); setA.push_back("12"); setA.push_back("08"); setA.push_back("17"); setA.push_back("09"); setA.push_back("14");
	setB.push_back("STR"); setB.push_back("CON"); setB.push_back("DEX"); setB.push_back("INT"); setB.push_back("WIS"); setB.push_back("CHA");
	
	A2BMenu m(stdscr, setA, "ROLLS", setB, "STATS", result, 2, 2);
	int i = m.select();
	m.~A2BMenu();
	clear();
	refresh();
	*/
/*
	initscr();
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	printw("test");

	refresh();
*/
	/*
	Map m;
	return 0;
	*/
}
