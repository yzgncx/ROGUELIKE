#include "globals.h"
#include "GameWindow.h"
#include <algorithm>


/*
 *
 *  ADVENTURE WINDOW METHODS
 *
 *
 */
AdventureWindow::AdventureWindow(Game* g) : m_Game(g)
{	
    m_Map = new Map(g);
    m_Player = m_Game->getPlayer();

    std::ifstream save;
    std::string path = "save/worldmap_test";
    save.open(path);

    m_display = newwin(0, 0, 0, 0);
    getmaxyx(m_display, m_max_y, m_max_x);
    m_max_y -= 7; //for HUD subwin

    m_HUD = subwin(m_display, 7, m_max_x, m_max_y, 0);
    whline(m_HUD, 0, m_max_x);

    m_MSG = subwin(m_HUD, 7, m_max_x - 45, m_max_y, 45);
    box(m_MSG,0,0);
      
    updateDisplay();
	
    touchwin(m_display);
    refresh();
    wrefresh(m_HUD);
    wrefresh(m_MSG);
}


AdventureWindow::~AdventureWindow()
{
	delete m_Map;
}

void AdventureWindow::runWindow()
{
  keypad(m_display, TRUE);
  while (true) {
    noecho();
    keypad(m_display, TRUE);

    handleInput(getch());
    updateEntities();
    updateInteractable();
    updateDisplay();
  }
}

void AdventureWindow::updateDisplay()
{
  short p_x, p_y;
  int ch;

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
  
  // print actor elements of the map
  // entities within range of player are printed in bold.
  for(go_mmap::iterator it = m_Map->m_go.begin(); it != m_Map->m_go.end(); it++) {
    for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it);
	s_it != m_Map->m_go.set_end(it); s_it++) {
      short o_x, o_y;
      (*s_it)->getXY(o_x,o_y);
      o_x -= x_offset;
      o_y -= y_offset;
      if (o_x >= 0 && o_x < m_max_x && o_y >= 0 && o_y < m_max_y) {
	if (std::find(m_interactable.begin(), m_interactable.end(), (*s_it))
	    != m_interactable.end()) {
	  attron(A_BOLD);
	}
	attron((*s_it)->getAttributes());
	mvprintw(o_y, o_x, "%c", (*s_it)->getAvatar());
	attroff((*s_it)->getAttributes());
	if (std::find(m_interactable.begin(), m_interactable.end(), (*s_it))
	    != m_interactable.end()) {
	  attroff(A_BOLD);
	}
      }
    }
  }
    
  //===================
  //update HUD elements
  std::string player_blurb = m_Player->getname();
  if(m_Player->gettitle() != "") {
    player_blurb += ", " + m_Player->gettitle();
  }
  player_blurb += " (level " + std::to_string(m_Player->getlevel()) + ")";
  //mvwprintw(m_HUD, 1, (m_max_x / 2) - (player_blurb.length() / 2), player_blurb.c_str());
  
  
  //HUD health bar
  mvwprintw(m_HUD, 2, 1, "HEALTH");
  std::string health_ratio = std::to_string(m_Player->getHealth()) +
    "/" + std::to_string(m_Player->getHealthMax());
  int health_ratio_len = health_ratio.length();
  int healthbar_len = (int)(21 * ((float)(m_Player->getHealth()) /
				  (float)(m_Player->getHealthMax())));
  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
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
  init_pair(4, COLOR_BLACK, COLOR_WHITE);
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

  //messages
  for(int i = m_messages.size() - 1, j = 1; i >= 0 && j < 6; i--) {
    mvwprintw(m_MSG, j, 1, m_messages[i].c_str());
    j++;
  }
  //================

  
  //print player's avatar.  this might be
  //replaced with a function call at some point
  init_pair(5, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(5));
  mvprintw(m_max_y / 2, m_max_x / 2, "@");
  attroff(COLOR_PAIR(5));
  
  
  touchwin(m_display);
  refresh();
  wrefresh(m_HUD);
}


//=======================================
//  UPDATE INTERACTABLE ENTITIES VECTOR
//=======================================
//
// This function takes into account the 'double-step' problem of W/E-ward
// player movements.
// Which entities are reachable is dependent on the direction the player is facing.
// In all four cases, all entities one step away are interactable.  In the case of
// W/E facing player, the entities two steps away are reachable i.f.f. there are no
// impenetrable objects one step away.
// 
void AdventureWindow::updateInteractable()
{
  short p_x, p_y;
  m_Map->getpXY(p_x,p_y);
  go_mmap::iterator it; 
  go_mmap::set_iterator s_it;
  bool impenetrable_entity = false;
  
  m_interactable.clear();

  switch (m_Player->getFacing())
  {
  case directions::N:
    it = m_Map->m_go.find_set(std::pair<short,short>(p_x, p_y-1));
    if (it != m_Map->m_go.end()) {
      for (s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it);s_it++) {
	m_interactable.push_back(*s_it);      
      }
    }
    break;
    
  case directions::W:
    it = m_Map->m_go.find_set(std::pair<short,short>(p_x-1, p_y));
    if (it != m_Map->m_go.end()) {
      for (s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it); s_it++) {
	m_interactable.push_back(*s_it);
	impenetrable_entity = (*s_it)->isPenetrable() | impenetrable_entity;
      }
    }
    if(impenetrable_entity) {
      break;
    }
    else {
      it = m_Map->m_go.find_set(std::pair<short,short>(p_x-2, p_y));
      if (it != m_Map->m_go.end()) {
	for (s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it); s_it++) {
	  m_interactable.push_back(*s_it);
	}
      }
    }
    break;
    
  case directions::S:
    it = m_Map->m_go.find_set(std::pair<short,short>(p_x, p_y+1));
    if (it != m_Map->m_go.end()) {
      for (s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it); s_it++) {
	m_interactable.push_back(*s_it);
      }
    }
    break;
    
  case directions::E:
    it = m_Map->m_go.find_set(std::pair<short,short>(p_x+1, p_y));
    if (it != m_Map->m_go.end()) {
      for (s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it); s_it++) {
	m_interactable.push_back(*s_it);
	impenetrable_entity = (*s_it)->isPenetrable() | impenetrable_entity;
      }
    }
    if(impenetrable_entity) {
      break;
    }
    else {
      it = m_Map->m_go.find_set(std::pair<short,short>(p_x+2, p_y));
      if (it != m_Map->m_go.end()) {
	for (s_it = m_Map->m_go.set_begin(it); s_it != m_Map->m_go.set_end(it); s_it++) {
	  m_interactable.push_back(*s_it);
	}
      }
    }
    break; 
  }  
}

//====================
//    HANDLE INPUT
//====================
//
//
void AdventureWindow::handleInput(char c)
{
  short p_x, p_y, n_x, n_y;
  
  m_Map->getpXY(p_x, p_y);
  n_x = p_x;
  n_y = p_y;
  
  switch (c) {
  //WASD cases (player turns face)
  case 'W':
    m_Player->setFacing(directions::N);
    break;
  case 'A':
    m_Player->setFacing(directions::W);
    break;
  case 'S':
    m_Player->setFacing(directions::S);
    break;
  case 'D':
    m_Player->setFacing(directions::E);
    break;
  
  //wasd cases (player turns face, then moves)  
  case 'w':
    m_Player->setFacing(directions::N);
    if (!canMove(directions::N)) {
      return;
    }
    n_y -= 1;
    break;
  case 'a':
    m_Player->setFacing(directions::W);
    if (!canMove(directions::W)) {
      return;
    }
    n_x -= 2;
    break;
  case 's':
    m_Player->setFacing(directions::S);
    if (!canMove(directions::S)) {
      return;
    }
    n_y += 1;
    break;
  case 'd':
    m_Player->setFacing(directions::E);
    if (!canMove(directions::E)) {
      return;
    }
    n_x += 2;
    break;

  // open InteractionSubwin 
  case ' ': {
    // DUMMY CODE
    std::vector<std::pair<std::string, std::string>> options_vec;
    options_vec.push_back(std::pair<std::string,std::string>("examine", "dummy"));
    PairScrollMenu selector(stdscr, options_vec, 15,55);
    switch(selector.select()) {
    case 0:
      m_messages.push_back("Doesn't look like anything to me.");
      break;
    }
    break;
  }
  }
  m_Player->setXY(n_x, n_y);
}

//========================
//  UPDATE ALL ENTITIES
//========================
//
void AdventureWindow::updateEntities()
{
  for(go_mmap::iterator it = m_Map->m_go.begin(); it != m_Map->m_go.end(); it++) {
    for(go_mmap::set_iterator s_it = m_Map->m_go.set_begin(it);
  	s_it != m_Map->m_go.set_end(it); s_it++) {
      short o_x, o_y;
      (*s_it)->performAction(); 
    }
  }
}

//======================================================
//  CHECK WHETHER PLAYER CAN MOVE IN A GIVEN DIRECTION
//======================================================
//
bool AdventureWindow::canMove(directions d)
{
  short p_x, p_y, n_x1, n_x2, n_y;
  m_Map->getpXY(p_x, p_y);
  n_x1 = n_x2 = p_x;
  n_y = p_y;
  
  switch(d) {
  case directions::N:
    n_y -= 1;
    break;
  case directions::W:
    n_x1 -= 1; n_x2 -= 2;
    break;
  case directions::S:
    n_y += 1;
    break;
  case directions::E:
    n_x1 += 1; n_x2 += 2;
    break;
  }

  switch(d) {
  case directions::W:
  case directions::E:
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
  case directions::N:
  case directions::S:
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


/*
 *
 * INTERACTION SUBWIN METHODS
 *
 *
 */

InteractionSubwin::InteractionSubwin(AdventureWindow* aw, WINDOW* p) : m_parent(p)
{
  
}
