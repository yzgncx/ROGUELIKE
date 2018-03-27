#include "Menu.h"

using namespace std::this_thread;
using namespace std::chrono;

/*
*
*  SLIDER MENU
*
*/
SliderMenu::SliderMenu(WINDOW* parent, std::string variable_name, int upper_lim, int lower_lim, int start, int delta_rate, int x_offset, int y_offset)
	: m_upper_lim(upper_lim), m_lower_lim(lower_lim), m_start(start), m_delta_rate(delta_rate), m_variable_name(variable_name)
{
	m_Menu = subwin(parent, 3, NumDigits(std::max(abs(upper_lim), abs(lower_lim))) + 4 + variable_name.length(), x_offset, y_offset);
	box(m_Menu, 0, 0);
	wmove(m_Menu, 1, 1);
	wprintw(m_Menu, (variable_name.c_str()));
	wprintw(m_Menu, ": %d", start);
	wrefresh(m_Menu);
}

SliderMenu::~SliderMenu()
{
}

int SliderMenu::select() const
{
	int ch;
	int counter(m_start);
	noecho();
	keypad(m_Menu, TRUE);
	while ((ch = getch()) != KEY_ENTER && ch != 10) {
		switch (ch) {
		case 'W':
		case 'w':
			if (counter < m_upper_lim) counter++;
			break;
		case 'S':
		case 's':
			if (counter > m_lower_lim) counter--;
			break;
		}
		wclear(m_Menu);
		box(m_Menu, 0, 0);
		wmove(m_Menu, 1, 1);
		wprintw(m_Menu, (m_variable_name.c_str()));
		wprintw(m_Menu, ": %d", counter);
		wrefresh(m_Menu);
		fflush(stdin);
	}
	return counter;
}

int SliderMenu::select(bool rand) const
{
	if (!rand) return SliderMenu::select();

	int ch;
	int counter(m_start);
	noecho();
	keypad(m_Menu, TRUE);
	while ((ch = getch()) != KEY_ENTER && ch != 10) {
		switch (ch) {
		case 'W':
		case 'w':
			if (counter < m_upper_lim) counter++;
			break;
		case 'S':
		case 's':
			if (counter > m_lower_lim) counter--;
			break;
		case 'R':
		case 'r':
			int i = 0;
			while (i < 10) {
				if (i < 5)
					sleep_for(milliseconds(50));
				else if (i < 8)
					sleep_for(milliseconds(100));
				else
					sleep_for(milliseconds(200));
				i++;
				counter = RandBetween(m_lower_lim, m_upper_lim);
				wclear(m_Menu);
				box(m_Menu, 0, 0);
				wmove(m_Menu, 1, 1);
				wprintw(m_Menu, (m_variable_name.c_str()));
				wprintw(m_Menu, ": %d", counter);
				wrefresh(m_Menu);
			}
		}
		wclear(m_Menu);
		box(m_Menu, 0, 0);
		wmove(m_Menu, 1, 1);
		wprintw(m_Menu, (m_variable_name.c_str()));
		wprintw(m_Menu, ": %d", counter);
		wrefresh(m_Menu);
	}
	return counter;
}

/*
*
*SCROLL MENU
*
*/
ScrollMenu::ScrollMenu(WINDOW* parent, std::vector<std::string> choices, int x_offset, int y_offset)
	: m_size(choices.size())
{
	int longest = 0;
	for (std::vector<std::string>::iterator it = choices.begin(); it != choices.end(); it++)
	{
		m_choices.push_back(*it);
		if (it->size() > static_cast<size_t>(longest))
			longest = it->size();
	}
	m_Menu = subwin(parent, choices.size() + 2, longest + 4, x_offset, y_offset);
	box(m_Menu, 0, 0);

	for (int i = 0; static_cast<size_t>(i) < m_choices.size(); i++)
	{
		wmove(m_Menu, i + 1, 2);
		if (i == 0) wattron(m_Menu, A_STANDOUT);
		wprintw(m_Menu, (m_choices[i]).c_str());
		if (i == 0) wattroff(m_Menu, A_STANDOUT);
	}
	wrefresh(m_Menu);
}

ScrollMenu::~ScrollMenu()
{
	delwin(this->m_Menu);
}

int ScrollMenu::select() const
{
	int ch, counter = 10000002;
	noecho();
	keypad(m_Menu, TRUE);
	while ((ch = getch()) != KEY_ENTER && ch != 10)
	{
		switch (ch)
		{
		case 'W':
		case 'w':
			counter--;
			break;
		case 'S':
		case 's':
			counter++;
			break;
		}
		int selected = (abs(counter % m_size));
		wclear(m_Menu);
		box(m_Menu, 0, 0);
		for (int i = 0; i < m_size; i++)
		{
			wmove(m_Menu, i + 1, 2);
			if (i == selected) wattron(m_Menu, A_STANDOUT);
			wprintw(m_Menu, (m_choices[i]).c_str());
			if (i == selected) wattroff(m_Menu, A_STANDOUT);
		}
		wrefresh(m_Menu);
		fflush(stdin);
	}
	wclear(m_Menu);
	wrefresh(m_Menu);
	return  (abs(counter % m_size));
}

int ScrollMenu::select(bool rand) const
{
	if (!rand) return ScrollMenu::select();

	int ch, counter = 0;
	noecho();
	keypad(m_Menu, TRUE);
	while ((ch = getch()) != KEY_ENTER && ch != 10) {
		switch (ch) {
		case 'W':
		case 'w':
			counter--;
			break;
		case 'S':
		case 's':
			counter++;
			break;
		case 'R':
		case 'r':
			int i = 0;
			while (i < 10) {
				if (i < 5)
					sleep_for(milliseconds(50));
				else if (i < 8)
					sleep_for(milliseconds(100));
				else
					sleep_for(milliseconds(200));
				i++;
				counter = Rand();
				int selected = (abs(counter % m_size));
				wclear(m_Menu);
				box(m_Menu, 0, 0);
				for (int i = 0; i < m_size; i++)
				{
					wmove(m_Menu, i + 1, 2);
					if (i == selected) wattron(m_Menu, A_STANDOUT);
					wprintw(m_Menu, (m_choices[i]).c_str());
					if (i == selected) wattroff(m_Menu, A_STANDOUT);
				}
				wrefresh(m_Menu);
			}
		}
		int selected = (abs(counter % m_size));
		wclear(m_Menu);
		box(m_Menu, 0, 0);
		for (int i = 0; i < m_size; i++)
		{
			wmove(m_Menu, i + 1, 2);
			if (i == selected) wattron(m_Menu, A_STANDOUT);
			wprintw(m_Menu, (m_choices[i]).c_str());
			if (i == selected) wattroff(m_Menu, A_STANDOUT);
		}
		wrefresh(m_Menu);
	}
	return (abs(counter % m_size));
}


/*
 *
 * PAIR SCROLL MENU
 *
 *
 */
PairScrollMenu::PairScrollMenu(WINDOW* parent,
			       std::vector<std::pair<std::string, std::string>> choices,
			       int x_offset, int y_offset)
    : m_size(choices.size())
{
    int longest = 0;
    for (std::vector<std::pair<std::string,std::string>>::iterator it = choices.begin();
	 it != choices.end(); it++) {
	    m_choices.push_back(*it);
	    int it_size = it->first.size() + it->second.size();
	    if (it_size > static_cast<size_t>(longest)) {
	        longest = it_size;
	    }
	}
    m_Menu = subwin(parent, choices.size() + 2, longest + 7, x_offset, y_offset);
    box(m_Menu, 0, 0);
    
    for (int i = 0; static_cast<size_t>(i) < m_choices.size(); i++) {
        wmove(m_Menu, i + 1, 2);
	if (i == 0) {
	  wprintw(m_Menu, "> ");
	}
	else {
	  wprintw(m_Menu, "  ");
	}
	wattron(m_Menu, A_BOLD);
	wprintw(m_Menu, (m_choices[i].first + " ").c_str());
	wattroff(m_Menu, A_BOLD);
	mvwprintw(m_Menu, i + 1, m_choices[i].first.size() + 5,
		  (m_choices[i].second).c_str());
    }
    wrefresh(m_Menu);
}

PairScrollMenu::~PairScrollMenu()
{
    delwin(this->m_Menu);
}

int PairScrollMenu::select() const
{
  int ch, counter = 10000002;
  noecho();
  keypad(m_Menu, TRUE);
  while ((ch = getch()) != KEY_ENTER && ch != 10) {
      switch (ch)
          {
	  case 'W':
	  case 'w':
	      counter--;
	      break;
	  case 'S':
	  case 's':
	      counter++;
	      break;
	  }
      int selected = (abs(counter % m_size));
      wclear(m_Menu);
      box(m_Menu, 0, 0);
      for (int i = 0; i < m_size; i++) {
	  wmove(m_Menu, i + 1, 2);
	  if (i == selected) {
	      wprintw(m_Menu, "> ");
	  }
	  else {
	      wprintw(m_Menu, "  ");
	  }
	  wattron(m_Menu, A_BOLD);
	  wprintw(m_Menu, (m_choices[i].first + " ").c_str());
	  wattroff(m_Menu, A_BOLD);
	  mvwprintw(m_Menu, i + 1, m_choices[i].first.size() + 5,
		    (m_choices[i].second).c_str());
      }
      wrefresh(m_Menu);
      fflush(stdin);
  }
  wclear(m_Menu);
  wrefresh(m_Menu);
  return  (abs(counter % m_size));
}

int PairScrollMenu::select(bool rand) const
{
    if (!rand) return PairScrollMenu::select();
    
    int ch, counter = 0;
    noecho();
    keypad(m_Menu, TRUE);
    while ((ch = getch()) != KEY_ENTER && ch != 10) {
        switch (ch) {
	case 'W':
	case 'w':
	    counter--;
	    break;
	case 'S':
	case 's':
	    counter++;
	    break;
	case 'R':
	case 'r':
	    int i = 0;
	    while (i < 10) {
	        if (i < 5)
		    sleep_for(milliseconds(50));
		else if (i < 8)
		    sleep_for(milliseconds(100));
		else
		    sleep_for(milliseconds(200));
		i++;
		counter = Rand();
		int selected = (abs(counter % m_size));
		wclear(m_Menu);
		box(m_Menu, 0, 0);
		for (int i = 0; i < m_size; i++) {
		    wmove(m_Menu, i + 1, 2);
		    if (i == selected) {
		        wprintw(m_Menu, "> ");
		    }
		    else {
		        wprintw(m_Menu, "  ");
		    }
		    wattron(m_Menu, A_BOLD);
		    wprintw(m_Menu, (m_choices[i].first + " ").c_str());
		    wattroff(m_Menu, A_BOLD);
		    mvwprintw(m_Menu, i + 1, m_choices[i].first.size() + 5,
			      (m_choices[i].second).c_str());
		    }
		wrefresh(m_Menu);
	    }
	}
	int selected = (abs(counter % m_size));
	wclear(m_Menu);
	box(m_Menu, 0, 0);
	for (int i = 0; i < m_size; i++) {
	    if(i == selected) {
	        wprintw(m_Menu, "> ");
	    }
	    else {
	        wprintw(m_Menu, "  ");
	    }
	    wmove(m_Menu, i + 1, 2);
	    wattron(m_Menu, A_BOLD);
	    wprintw(m_Menu, (m_choices[i].first + " ").c_str());
	    wattroff(m_Menu, A_BOLD);
	    wmove(m_Menu, i + 1, m_choices[i].first.size() + 3);
	    wprintw(m_Menu, (m_choices[i].second).c_str());
	}
	wrefresh(m_Menu);
    }
    return (abs(counter % m_size));
}



/*
*
*
*  A2B MENU
*
*/
A2BMenu::A2BMenu(WINDOW* parent, std::vector<std::string> setA,
		 std::string setA_header, std::vector<std::string> setB,
		 std::string setB_header, std::vector<int> &result,
		 int x_offset, int y_offset)
	: m_setA_header(setA_header), m_setB_header(setB_header),
	  m_setA_size(setA.size()), m_setB_size(setB.size()),
	  m_result(&result)
{
  m_longest_in_setA = m_longest_in_setB = 0;
  for (std::vector<std::string>::iterator it = setB.begin(); it != setB.end(); it++) {	//build setB
    m_setB.push_back(setBItem(*it));
    if (it->size() > m_longest_in_setB) {
      m_longest_in_setB = it->size();
    }
  }
  m_setB.push_back(setBItem(""));

  for (std::vector<std::string>::iterator it = setA.begin(); it != setA.end(); it++) {	//build setA
    m_setA.push_back(setAItem(*it, m_setB.size()-1));
    if (it->size() > m_longest_in_setA) {
      m_longest_in_setA = it->size();
    }
  }
  m_setA_size = m_setA.size();
  m_setB_size = m_setB.size();
  m_width = std::max(m_longest_in_setA, m_setA_header.size()) +
            std::max(m_longest_in_setB, m_setB_header.size()) + 20;
  m_space = std::string(m_width - 4, ' ');
  
  m_Menu = subwin(parent, m_setA_size + 4, m_width, x_offset, y_offset);
  box(m_Menu, 0, 0);
  
  mvwprintw(m_Menu, 1, 2, m_setA_header.c_str());      	                //print setA header in subwin
  int setB_header_offset = m_width - (m_setB_header.size() + 2); 
  mvwprintw(m_Menu, 1, setB_header_offset, m_setB_header.c_str());	//print setB header in subwin
  
  for (size_t i = 0; i < m_setA.size(); i++) {
    wmove(m_Menu, i + 3, 2);
    if (i == 0) {
	wattron(m_Menu, A_STANDOUT);
	wprintw(m_Menu, m_space.c_str());       //highlighted whitespace
	wmove(m_Menu, i + 3, 2);
    }
    wprintw(m_Menu, (m_setA[i].name).c_str());
    if (i == 0) {       			//print arrows
      int r_arrow_offset = (m_width - 3);
      int l_arrow_offset = (m_width - (4 + m_longest_in_setB));
      mvwprintw(m_Menu, 3, r_arrow_offset, ">");
      mvwprintw(m_Menu, 3, l_arrow_offset, "<");
      
      wattroff(m_Menu, A_STANDOUT);            //turn off highlighting
    }
  }
  wrefresh(m_Menu);
}


A2BMenu::~A2BMenu()
{
}

int A2BMenu::select()
{
  int ch, counter = 1000092, selected = 0;
  /*
    ch is set to this large value so that the behavior of up/down
    commands remains consistent.  direction of up/down selection 
    reverses when counter crosses zero.  
  */
  bool valid_state = false;
  noecho();
  while (((ch = getch()) != KEY_ENTER && ch != 10) || !valid_state) {
    switch (ch) {
    case 'W':	// move setA selector up
    case 'w':	//
      counter--;
      break;
    case 'S':	// move setA selector down
    case 's':	//
      counter++;
      break;
    case 'A':	// change selected setB for selected setA left;
    case 'a':	//
      if (m_setA[selected].mapped_to > 0) {	
	m_setB[m_setA[selected].mapped_to].mapped = false;
	m_setA[selected].mapped_to = (m_setA[selected].mapped_to - 1) % m_setB_size;
      }
      else if (m_setA[selected].mapped_to == 0) {
	m_setB[m_setA[selected].mapped_to].mapped = false;
	m_setA[selected].mapped_to = m_setB_size-1;
      }
      else
	m_setA[selected].mapped_to = m_setB_size - 1;
      while (m_setB[m_setA[selected].mapped_to].mapped && m_setA[selected].mapped_to != m_setB.size() - 1) {
	if (m_setA[selected].mapped_to >= 0) {
	  m_setA[selected].mapped_to--;
	}
	if (m_setA[selected].mapped_to < 0) {
	  m_setA[selected].mapped_to = m_setB_size - 1;
	}
      }
      m_setB[m_setA[selected].mapped_to].mapped = true;
      break;
    case 'D':	// change selected setB for selected setA right;
    case 'd':	//
      if ((m_setA[selected].mapped_to) < static_cast<int>(m_setB_size - 1)) {
	  m_setB[m_setA[selected].mapped_to].mapped = false;
	  m_setA[selected].mapped_to = (m_setA[selected].mapped_to + 1) % m_setB_size;
	}
      else if ((m_setA[selected].mapped_to) == (m_setB_size - 1)) {
	  m_setB[m_setA[selected].mapped_to].mapped = false;
	  m_setA[selected].mapped_to = 0;
	}
      while (m_setB[m_setA[selected].mapped_to].mapped && m_setA[selected].mapped_to != m_setB.size() - 1) {
	if ((m_setA[selected].mapped_to) <= static_cast<int>(m_setB_size - 1)) {
	  m_setA[selected].mapped_to++;
	}
	if ((m_setA[selected].mapped_to) > static_cast<int>(m_setB_size - 1)) {
	    m_setA[selected].mapped_to = 0;
	}
      }
      m_setB[m_setA[selected].mapped_to].mapped = true;
      break;      
    }
      
      selected = (abs(counter % static_cast<int>(m_setA_size)));
      wclear(m_Menu);
      box(m_Menu, 0, 0);
      
      mvwprintw(m_Menu, 1, 2, m_setA_header.c_str());										//print setA header in menu subwindow
      int setB_header_offset = m_width - (m_setB_header.size() + 2);
      mvwprintw(m_Menu, 1, setB_header_offset, m_setB_header.c_str());	//print setB header in submenu
      for (size_t i = 0; i < m_setA_size; i++) {
	wmove(m_Menu, i + 3, 2);
	if (i == selected) {
	  wattron(m_Menu, A_STANDOUT);
	  wprintw(m_Menu, m_space.c_str());
	  int r_arrow_offset =  (m_width - 3);
	  mvwprintw(m_Menu, (i+3), r_arrow_offset, ">");
	  mvwprintw(m_Menu, (i+3), (m_width - (4 + m_longest_in_setB)), "<");
	  wmove(m_Menu, i + 3, 2);
	}
	wprintw(m_Menu, (m_setA[i].name).c_str());
	if (m_setA[i].mapped_to >= 0 && m_setA[i].mapped_to < static_cast<int>(m_setB.size())) {
	  int offset = m_width - (m_setB[m_setA[i].mapped_to].name.size() + 3);
	  mvwprintw(m_Menu, i + 3, offset, m_setB[m_setA[i].mapped_to].name.c_str());
	  //print setB item for given setA item
	}
	if (i == selected) { //print arrows
	  int r_arrow_offset = (m_width - 3);
	  int l_arrow_offset = (m_width - (4 + m_longest_in_setB));
	  mvwprintw(m_Menu, 3+i, r_arrow_offset, ">");
	  mvwprintw(m_Menu, 3+i, l_arrow_offset, "<");
	  wattroff(m_Menu, A_STANDOUT);
	}
      }
      wrefresh(m_Menu);

      //
      //check valid state:
      //	valid_state == true iff for all x in setA, x is 
      //	mapped to, or for all x in setB, x is mapped.
      bool A_complete = true, B_complete = true;
      for (size_t i = 0; i < m_setA_size; i++) {
	if (m_setA[i].mapped_to == m_setA_size) {
	  A_complete = false;
	}
      }
      for (size_t i = 0; i < m_setB_size; i++) {
	if (!m_setB[i].mapped) {
	  B_complete = false;
	}
      }
      if (A_complete || B_complete) {
	valid_state = true;
      }
  }

  //
  //generate result vector
  std::vector<int> result;
  for (size_t i = 0; i < m_setA_size; i++) {
    result.push_back(m_setA[i].mapped_to);
  }
  m_result->swap(result);	//use swap function as a failsafe, so we know that reallocation will take place.
  return 0;
}


//
//TODO: write random gen select code
//
int A2BMenu::select(bool rand)
{
	return select();
}
