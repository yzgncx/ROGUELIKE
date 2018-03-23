#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "Menu.h"
#include "Actor.h"
#include "Stats.h"
#include <ncurses.h>
#include "globals.h"

class Game
{
public:
	Game();
	~Game();

	void newGame();
	void generatestats(WINDOW* parent, int x_offset, int y_offset, std::vector<Attribute> &statset);
	void loadGame();
	int checkName(std::string name);

	std::string getFilename();
	Player* getPlayer();
private:
	std::string m_filename;
	Player* m_Player;
};

#endif // !GAME_H
