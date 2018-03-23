#include "Map.h"
#include "globals.h"

Map::Map(Game* g) : m_Game(g)
{
  m_Player = m_Game->getPlayer();
  char map_file[] = "world/map_1";
  std::ifstream map;
  map.open(map_file);
  
  std::vector<char> line;
  if (map.bad()) {
    throw BAD_FILE;
  }
  while (!map.eof()) {
    char c;
    map.get(c);
    if (c == '\n' || map.eof()) {
      std::vector<char> tmp;
      m_charmap.push_back(line);
      line.swap(tmp);
    }
    else {
      line.push_back(c);
    }
  }
  map.close();
	

	//
	// read from entities file
	//
	// I've chosen to use a more c-like style for this function, only since fscan
	// handles this sort of reading so elegantly. fscan's regex-like syntax also
	// makes it trivial to line up the numbers in the file being read from.
  char entities_file[] = "./world/entities_test";
  FILE* entities = fopen(entities_file, "r");
  if (entities == NULL) {
    throw BAD_FILE;
  }
  
  while (true) {	
    short type = 0, sub = 0, x = 0, y = 0;
    if (fscanf(entities, "%hu %hu %hu %hu", &type, &sub, &x, &y) == 4) {
      if (type == PLAYER) {
	m_Player->setXY(x, y);
      }
      else {
	Actor *tmp;
	switch (type) {
	  //	case ITEM:
	  //		break;
	case MOB:
	  tmp = new Mob(sub, x, y, g, 1, 0, 0);
	  m_go.insert(std::pair<short,short>(x,y),tmp);
	  break;
	case TERRAIN:
	  tmp = new Terrain(sub, x, y, g, 1, 0);
	  m_go.insert(std::pair<short,short>(x,y),tmp);
	  break;
	}
      }
    }
    else {
      break;
    }
  }
}


Map::~Map()
{
}

bool Map::isLoaded() { return m_loaded; }

unsigned char Map::getChar(short y, short x)
{
  if (x < 0 || y < 0 || y >= (int)(m_charmap.size()) || x >= (int)(m_charmap[y].size())) {
    return '\0';
  }
  return m_charmap[y][x];
}

size_t Map::getCharmapSize()
{
  return m_charmap.size();
}

size_t Map::getCharmapSize(unsigned int row)
{
  if (row >= m_charmap.size()) {
    throw BAD_RANGE;
  }
  return m_charmap[row].size();
}

void Map::getpXY(short &x, short &y)
{
  m_Player->getXY(x, y);
}
