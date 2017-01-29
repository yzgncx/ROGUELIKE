#include "GameWindow.h"

AdventureWindow::AdventureWindow(Game* g) : m_Game(g)
{
	
	m_Map = new Map(g);
	m_Player = m_Game->getPlayer();

	std::ifstream save;
	std::string path = "save\\worldmap_test.txt";
	save.open(path);

	initscr();

	m_display = newwin(0, 0, 0, 0);
	getmaxyx(m_display, m_max_y, m_max_x);
	m_max_y -= 7; //for HUD subwin
	m_HUD = subwin(m_display, 5, m_max_x, m_max_y, 0);

	short p_x, p_y;
	m_Map->getpXY(p_x, p_y);
	int y_offset = (int)(p_y)-(m_max_y / 2), x_offset = (int)(p_x)-(m_max_x / 2);

	int y_size = m_Map->getCharmapSize();
	for (int y = 0; y < m_max_y && y < y_size; y++)
	{
		int x_size = m_Map->getCharmapSize(y);
		for (int x = 0; x < m_max_x && x < x_size; x++)
		{
			unsigned char c = m_Map->getChar(y_offset + y, x_offset + x);
			if (c == '\0') c = '#';
			mvprintw(y, x, "%c", c);
		}
	}

	mvprintw(m_max_y / 2, m_max_x / 2, "@");

	int size = m_Map->getGraphObjSize();
	for (int i = 0; i < size; i++)
	{
		GraphObject* s = (GraphObject*)(m_Map->getGraphObject(i));
		mvprintw(y_offset, x_offset, "%c", s->getAvatar());
	}

	whline(m_HUD, 0, m_max_x);
	mvwprintw(m_HUD, 1, 1, "hello subwin");
	touchwin(m_display);
	refresh();
	wrefresh(m_HUD);
}


AdventureWindow::~AdventureWindow()
{
	delete m_Map;
}

void AdventureWindow::runWindow()
{
	short p_x, p_y;
	int ch;

	noecho();
	keypad(m_display, TRUE);
	while (ch = getch()) {
		m_Map->getpXY(p_x, p_y);
		if (!canMove(ch)) continue;
		switch (ch)
		{
		case 'w':
			m_Player->setXY(p_x, p_y - 1);
			break;
		case 'a':
			m_Player->setXY(p_x - 2, p_y);
			break;
		case 's':
			m_Player->setXY(p_x, p_y + 1);
			break;
		case 'd':
			m_Player->setXY(p_x + 2, p_y);
			break;
		}

		m_Player->getXY(p_x, p_y);
		int y_offset = p_y - (m_max_y / 2), x_offset = p_x - (m_max_x / 2);
		int y_size = m_Map->getCharmapSize();
		for (int y = 0; y < m_max_y && y < y_size; y++)
		{
			int x_size = m_Map->getCharmapSize(y);
			for (int x = 0; x < m_max_x && x < x_size; x++)
			{
				unsigned char c = m_Map->getChar(y_offset + y, x_offset + x);
				if (c == '\0') c = '#';
				mvprintw(y, x, "%c", c);
			}
		}

		int size = m_Map->getGraphObjSize();
		for (int i = 0; i < size; i++)
		{
			short o_x, o_y;
			GraphObject* s = (GraphObject*)(m_Map->getGraphObject(i));
			s->getXY(o_x, o_y);
			o_x -= x_offset;
			o_y -= y_offset;
			if (o_x >= 0 && o_x < m_max_x && o_y >= 0 && o_y < m_max_y)
				mvprintw(o_y, o_x, "%c", s->getAvatar());
		}

		//print player's avatar.  this might be
		//replaced with a function call at some point
		mvprintw(m_max_y / 2, m_max_x / 2, "@");

		mvwprintw(m_HUD, 1, 1, "hello subwin");
		touchwin(m_display);
		refresh();
		wrefresh(m_HUD);
	}
}

bool AdventureWindow::canMove(char c)
{
	short p_x, p_y;
	m_Map->getpXY(p_x, p_y);
	switch (c)
	{
	case 'w':
		for (size_t i = 0; i < m_Map->getGraphObjSize(); i++)
		{
			short o_x, o_y;
			GraphObject* gp = (GraphObject*)(m_Map->getGraphObject(i));
			gp->getXY(o_x, o_y);
			if ((!gp->isPenetrable() && o_x == p_x && o_y == p_y - 1))
				return false;
		}
		if (m_Map->getChar(p_y - 1, p_x) == '\0')
			return false;
		break;
	case 'a':
		for (size_t i = 0; i < m_Map->getGraphObjSize(); i++)
		{
			short o_x, o_y;
			GraphObject* gp = (GraphObject*)(m_Map->getGraphObject(i));
			gp->getXY(o_x, o_y);
			if (!gp->isPenetrable() && (o_x == p_x - 1 || o_x == p_x - 2) && o_y == p_y)
				return false;
		}

		if (m_Map->getChar(p_y, p_x - 1) == '\0' || m_Map->getChar(p_y, p_x - 2) == '\0')
			return false;
		break;
	case 's':
		for (size_t i = 0; i < m_Map->getGraphObjSize(); i++)
		{
			short o_x, o_y;
			GraphObject* gp = (GraphObject*)(m_Map->getGraphObject(i));
			gp->getXY(o_x, o_y);
			if (!gp->isPenetrable() && o_x == p_x && o_y == p_y + 1)
				return false;
		}
		if (m_Map->getChar(p_y + 1, p_x) == '\0')
			return false;
		break;
	case 'd':
		for (size_t i = 0; i < m_Map->getGraphObjSize(); i++)
		{
			short o_x, o_y;
			GraphObject* gp = (GraphObject*)(m_Map->getGraphObject(i));
			gp->getXY(o_x, o_y);
			if (!gp->isPenetrable() && (o_x == p_x + 1 || o_x == p_x + 2) && o_y == p_y)
				return false;
		}
		if (m_Map->getChar(p_y, p_x + 1) == '\0' || m_Map->getChar(p_y, p_x + 2) == '\0')
			return false;
		break;
	default:
		break;
	}
	return true;
}