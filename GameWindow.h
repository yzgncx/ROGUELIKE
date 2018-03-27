#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <vector>
#include <fstream>
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
	bool canMove(directions d);
private:
	WINDOW* m_display;
	WINDOW* m_HUD;
	WINDOW* m_MSG;
	std::vector<std::string> m_messages;
	
	Game* m_Game;
	Map* m_Map;
	Player* m_Player;
	std::vector<Actor*> m_interactable;

	int m_max_x;
	int m_max_y;
};

class InteractionSubwin
{
  public:
  InteractionSubwin(AdventureWindow* aw, WINDOW* p);

 private:
    WINDOW* m_parent;
    WINDOW* m_subwin;
};




#endif // !GAMEWINDOW_H

