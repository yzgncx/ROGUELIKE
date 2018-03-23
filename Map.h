#ifndef MAP_H
#define MAP_H
#include <list>
#include <map>
#include <vector>
#include "Game.h"
#include "Actor.h"
#include "globals.h"


class Map
{
public:
	Map(Game* g);  //will require filepath of some sort
	~Map();
	
	bool isLoaded();
	unsigned char getChar(short x, short y);
	size_t getCharmapSize();
	size_t getCharmapSize(unsigned int row);

	void getpXY(short &x, short &y);

	/*probably a better place for this, but this simplifies things*/
	go_mmap m_go; //m_graph_objects
	
private:
	bool m_loaded;
	Game* m_Game;

	struct { short x, y; } m_pcoord;
	std::vector<std::vector<char>> m_charmap;
	Actor* m_Player;

	
};



#endif
