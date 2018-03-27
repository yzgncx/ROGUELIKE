#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <vector>
#include <fstream>
#include <ncurses.h>
#include "globals.h"
#include "Actor.h"
#include "Game.h"
#include "Map.h"


class AdventureWindow
{
public:
	AdventureWindow(Game* g);
	~AdventureWindow();

	void runWindow();
	void updateDisplay();
	void updateInteractable();
	void handleInput(char c);
	void updateEntities();
	bool canMove(char c);
private:
	WINDOW* m_display;
	WINDOW* m_HUD;

	Game* m_Game;
	Map* m_Map;
	Player* m_Player;
	std::vector<Actor*> m_interactable;

	
	int m_max_x;
	int m_max_y;
};




#endif // !GAMEWINDOW_H

