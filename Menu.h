
#ifndef MENU_H
#define MENU_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "curses.h"
#include "globals.h"
#include <chrono> // for sleep cycles in rand
#include <thread> // ditto


/*
*
*  SLIDER MENU
*
*/
class SliderMenu
{
public:
	SliderMenu(WINDOW* parent, std::string variable_name, int upper_lim, int lower_lim, int start, int delta_rate, int x_offset, int y_offset);
	~SliderMenu();
	virtual int select() const;
	virtual int select(bool rand) const;

private:
	std::string m_variable_name;
	WINDOW* m_Menu;
	int m_upper_lim, m_lower_lim, m_start, m_delta_rate;
};


/*
*
*  SCROLL MENU
*
*/
class ScrollMenu
{
public:
	ScrollMenu(WINDOW* parent, std::vector<std::string> choices, int x_offset, int y_offset);
	~ScrollMenu();
	virtual int select() const;
	virtual int select(bool rand) const;

private:
	std::vector<std::string> m_choices;
	int m_size;
	WINDOW* m_Menu;
};


/*
*
*  A2B MENU
*	allows user to map items in a set_A to items in a set_B
*	returns vector<int> of size set_A, such that each index 
*	of return_vector corresponds to the same index in set_A
*	and such that the value at return_vector[i] is equal to
*	the index of an item in set_B.
*
*/

class A2BMenu
{
public:

	A2BMenu(WINDOW* parent, std::vector<std::string> setA, std::string setA_header, std::vector<std::string> setB, std::string setB_header, std::vector<int> &result, int x_offset, int y_offset);
	~A2BMenu();
	virtual int select();
	virtual int select(bool rand);

	struct setAItem {
		setAItem(std::string n, int m = -1) { mapped_to = m; name = n; }
		std::string name;
		int mapped_to;
	};

	struct setBItem {
		setBItem(std::string n, bool m = false) { mapped = m; name = n; }
		std::string name;
		bool mapped;
	};

private:


	WINDOW* m_Menu;
	std::vector<int>* m_result;
	std::string m_setA_header;
	std::string m_setB_header;
	std::vector<setAItem> m_setA;
	std::vector<setBItem> m_setB;
	size_t m_setA_size;
	size_t m_setB_size;
	unsigned m_longest_in_setA;
	unsigned m_longest_in_setB;
};


#endif // !MENU_H