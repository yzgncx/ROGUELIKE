#include "Map.h"



Map::Map( Game* g ) : m_Game(g)
{
	m_Player = m_Game->getPlayer();
	char* filename = "world\\map_1.txt";
	std::ifstream map;
	map.open(filename);

	std::vector<char> line;
	if (map.bad()) throw BAD_FILE;
	while (!map.eof())
	{
		char c;
		map.get(c);
		if (c == '\n' || map.eof())
		{
			std::vector<char> tmp;
			m_charmap.push_back(line);
			line.swap(tmp);
		} else
		line.push_back(c);
	}
	map.close();
	

	//
	// read from entities file
	//
	// I've chosen to use a more c-like style for this function, only since fscan
	// handles this sort of reading so elegantly. fscan's regex-like syntax also
	// makes it trivial to line up the numbers in the file being read from.
	filename = "world\\entities_test.txt";
	FILE* entities;
	errno_t err;
	if (err = fopen_s(&entities, filename, "rb") != 0) throw BAD_FILE;
	while (true)
	{
		
		short type = 0, sub = 0, x = 0, y = 0;
		if (fscanf_s(entities, "%hu %hu %hu %hu", &type, &sub, &x, &y) == 4)
		{
			if (type == PLAYER) {
				m_Player->setXY(x, y);
			}
			else {
				switch (type)
				{
			//	case ITEM:
			//		break;
			//	case MOB:
			//		break;
				case TERRAIN:
					m_graph_objects.push_back(Terrain(sub, x, y, g, 1, 0)); //temporary
					break;
				}
			}
		}
		else
			break;
	}
}


Map::~Map()
{
}

bool Map::isLoaded() { return m_loaded; }

unsigned char Map::getChar(short y, short x)
{
	if (x < 0 || y < 0 || y >= (int)(m_charmap.size()) || x >= (int)(m_charmap[y].size()))
		return '\0';
	return m_charmap[y][x];
}

size_t Map::getCharmapSize()
{
	return m_charmap.size();
}

size_t Map::getCharmapSize(unsigned int row)
{
	if (row >= m_charmap.size())
		throw BAD_RANGE;
	return m_charmap[row].size();
}


void* Map::getGraphObject(int i)
{
	if (i < 0 || i >= (int)(m_graph_objects.size()))
		return NULL;
	switch (m_graph_objects[i].getType())
	{
	case TERRAIN:
		return &m_graph_objects[i];
	case MOB:
	case ITEM:
	default:
		break;
	}
	return NULL;
}

size_t Map::getGraphObjSize()
{
	return m_graph_objects.size();
}

void Map::getpXY(short &x, short &y)
{
	m_Player->getXY(x, y);
}