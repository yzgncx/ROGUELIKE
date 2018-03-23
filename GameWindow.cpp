#include "globals.h"
#include "GameWindow.h"

AdventureWindow::AdventureWindow(Game* g) : m_Game(g)
{	
  m_Map = new Map(g);
  m_Player = m_Game->getPlayer();

  std::ifstream save;
  std::string path = "save/worldmap_test";
  save.open(path);
  
  initscr();
  
  m_display = newwin(0, 0, 0, 0);
  getmaxyx(m_display, m_max_y, m_max_x);
  m_max_y -= 7; //for HUD subwin
  m_HUD = subwin(m_display, 5, m_max_x, m_max_y, 0);
  
  short p_x, p_y;
  m_Map->getpXY(p_x, p_y);
  int y_offset = (int)(p_y)-(m_max_y / 2),
      x_offset = (int)(p_x)-(m_max_x / 2);

  int y_size = m_Map->getCharmapSize();
  for (int y = 0; y < m_max_y && y < y_size; y++) {
    int x_size = m_Map->getCharmapSize(y);
    for (int x = 0; x < m_max_x && x < x_size; x++) {
      unsigned char c = m_Map->getChar(y_offset + y, x_offset + x);
      if (c == '\0') c = '#';
      mvprintw(y, x, "%c", c);
    }
  }

  mvprintw(m_max_y / 2, m_max_x / 2, "@");
  
  for(go_mmap::iterator it = m_Map->m_go.begin(); it != m_Map->m_go.end(); it++) { 
    for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it); s_it++) {
      mvprintw(y_offset, x_offset, "%c", (*s_it)->getAvatar());
    }
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
  while (true) {
    updatePlayer();

    m_Player->getXY(p_x, p_y);
    int y_offset = p_y - (m_max_y / 2), x_offset = p_x - (m_max_x / 2);
    int y_size = m_Map->getCharmapSize();
    for (int y = 0; y < m_max_y && y < y_size; y++) {
      int x_size = m_Map->getCharmapSize(y);
      for (int x = 0; x < m_max_x && x < x_size; x++) {
	unsigned char c = m_Map->getChar(y_offset + y, x_offset + x);
	if (c == '\0') {
	  c = '#';
	}	
	mvprintw(y, x, "%c", c);
      }
    }

    for(go_mmap::iterator it = m_Map->m_go.begin(); it != m_Map->m_go.end(); it++) {
      for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it); s_it++) {
	short o_x, o_y;
	(*s_it)->getXY(o_x,o_y);
	o_x -= x_offset;
	o_y -= y_offset;
	if (o_x >= 0 && o_x < m_max_x && o_y >= 0 && o_y < m_max_y) {
	  mvprintw(o_y, o_x, "%c", (*s_it)->getAvatar());
	}
      }
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

void AdventureWindow::updatePlayer()
{
  short p_x, p_y, n_x, n_y;
  int ch;
  
  noecho();
  keypad(m_display, TRUE);
  ch = getch();
  m_Map->getpXY(p_x, p_y);
  n_x = p_x;
  n_y = p_y;
  
  if (!canMove(ch)) {
    return;
  }
  switch (ch) {
  case 'w':
    n_y -= 1;
    break;
  case 'a':
    n_x -= 2;
    break;
  case 's':
    n_y += 1;
    break;
  case 'd':
    n_x += 2;
    break;
  }
  m_Player->setXY(n_x, n_y);
}


void AdventureWindow::updateEntities(){}

bool AdventureWindow::canMove(char c)
{
  short p_x, p_y, n_x1, n_x2, n_y;
  m_Map->getpXY(p_x, p_y);
  n_x1 = n_x2 = p_x;
  n_y = p_y;
  
  switch(c) {
  case 'w':
    n_y -= 1;
    break;
  case 'a':
    n_x1 -= 1; n_x2 -= 2;
    break;
  case 's':
    n_y += 1;
    break;
  case 'd':
    n_x1 += 1; n_x2 += 2;
    break;
  }

  switch(c) {
  case 'a':
  case 'd':
    {
    go_mmap::iterator it2 = m_Map->m_go.find_set(std::pair<short,short>(n_x2,n_y));
    if(it2 != m_Map->m_go.end()) {
      for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it2); s_it != m_Map->m_go.set_end(it2); s_it++) {
	if(!((*s_it)->isPenetrable())) {
	  return false;
	}
      }
    }
    }
  case 'w':
  case 's':
    {
    go_mmap::iterator it = m_Map->m_go.find_set(std::pair<short,short>(n_x1,n_y));
    if(it != m_Map->m_go.end()) {
      for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it); s_it++) {
	if(!((*s_it)->isPenetrable())) {
	  return false;
	}
      }
    } 
    break;
    }
  }
  
  if (m_Map->getChar(n_y, n_x1) == '\0' || m_Map->getChar(n_y, n_x2) == '\0') {
    return false;
  }

  return true;
}
