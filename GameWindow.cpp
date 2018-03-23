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
  
    for(go_mmap::iterator it = m_Map->m_go.begin();
	it != m_Map->m_go.end(); it++) { 
        for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it);
	    s_it != m_Map->m_go.set_end(it); s_it++) {
	  mvprintw(y_offset, x_offset, "%c", (*s_it)->getAvatar());
	}
    }
	
    whline(m_HUD, 0, m_max_x);
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

    //print static elements of map
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
    
    //print dynamic elements of the map 
    for(go_mmap::iterator it = m_Map->m_go.begin(); it != m_Map->m_go.end(); it++) {
      for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it);
	  s_it != m_Map->m_go.set_end(it); s_it++) {
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


    //update HUD elements
    std::string player_blurb = m_Player->getname();
    if(m_Player->gettitle() != "") {
      player_blurb += ", " + m_Player->gettitle();
    }
    player_blurb += " (level " + std::to_string(m_Player->getlevel()) + ")";
    mvwprintw(m_HUD, 1, (m_max_x / 2) - (player_blurb.length() / 2), player_blurb.c_str());


    //HUD health bar
    mvwprintw(m_HUD, 2, 1, "HEALTH");
    std::string health_ratio = std::to_string(m_Player->getHealth()) +
                               "/" + std::to_string(m_Player->getHealthMax());
    int health_ratio_len = health_ratio.length();
    int healthbar_len = (int)(21 * ((float)(m_Player->getHealth()) /
				    (float)(m_Player->getHealthMax())));
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    wattron(m_HUD, COLOR_PAIR(1));
    for(int i = 0; i < 21; i++) {
      char c;
      if(i >= (10 - (health_ratio_len / 2)) && i <= (10 + (health_ratio_len / 2))) {
	c = health_ratio[i - (10 - (health_ratio_len / 2))];
      }
      else {
	c = ' ';
      }
      mvwaddch(m_HUD, 2, i + 8, c);
      if(i > healthbar_len) {
	wattroff(m_HUD, COLOR_PAIR(1));
	wattron(m_HUD, COLOR_PAIR(2));
      }
    }
    wattroff(m_HUD, COLOR_PAIR(2));
    wattroff(m_HUD, COLOR_PAIR(1));

    //HUD mana bar
    std::string mana_ratio = std::to_string(m_Player->getMana()) +
                               "/" + std::to_string(m_Player->getManaMax());
    int mana_ratio_len = mana_ratio.length();
    int manabar_len = (int)(21 * ((float)(m_Player->getMana()) /
				    (float)(m_Player->getManaMax())));
    init_pair(3, COLOR_BLACK, COLOR_CYAN);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    wattron(m_HUD, COLOR_PAIR(3));
    for(int i = 0; i < 21; i++) {
      char c;
      if(i >= (10 - (mana_ratio_len / 2)) && i <= (10 + (mana_ratio_len / 2))) {
	c = mana_ratio[i - (10 - (mana_ratio_len / 2))];
      }
      else {
	c = ' ';
      }
      mvwaddch(m_HUD, 4, i + 8, c);
      if(i > manabar_len) {
	wattroff(m_HUD, COLOR_PAIR(3));
	wattron(m_HUD, COLOR_PAIR(4));
      }
    }
    wattroff(m_HUD, COLOR_PAIR(3));
    wattroff(m_HUD, COLOR_PAIR(4));

    mvwprintw(m_HUD, 4, 1, "MANA  ");
    
    //
    //
    
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
      for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it2);
	  s_it != m_Map->m_go.set_end(it2); s_it++) {
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
      for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it);
	  s_it != m_Map->m_go.set_end(it); s_it++) {
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
