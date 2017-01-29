#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <vector>
#include <fstream>
#include "curses.h"
#include "globals.h"
#include "Game.h"
#include "Map.h"

class AdventureWindow
{
public:
	AdventureWindow(Game* g);
	~AdventureWindow();

	void runWindow();
	bool canMove(char c);
private:
	WINDOW* m_display;
	WINDOW* m_HUD;

	Game* m_Game;
	Map* m_Map;
	Player* m_Player;

	int m_max_x;
	int m_max_y;
};




#endif // !GAMEWINDOW_H

