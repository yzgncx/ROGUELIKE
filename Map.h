#ifndef MAP_H
#define MAP_H
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
	size_t Map::getCharmapSize();
	size_t Map::getCharmapSize(unsigned int row);
	void* Map::getGraphObject(int i);
	size_t Map::getGraphObjSize();
	
	void getpXY(short &x, short &y);

private:
	bool m_loaded;
	Game* m_Game;

	struct { short x, y; } m_pcoord;
	std::vector<std::vector<char>> m_charmap;
	Actor* m_Player;
	std::vector<Actor> m_graph_objects;
};

#endif