#ifndef STATS_H
#define STATS_H

#include "Attribute.h"
#include "globals.h"
#include <ncurses.h>
#include "Menu.h"
#include <vector>

class Stats
{
public:
	Stats(WINDOW* parent, int x_offset, int y_offset);
	~Stats();

	void rollStats_houseRules( WINDOW* parent, int x_offset,
				   int y_offset, std::vector<int> &result);
	// 4d6. reroll 1s 
	// if 4*1, 21;
	// otherwise throw out low roll
	// 

	void rollStats_standard(WINDOW* parent, int x_offset,
				int y_offset, std::vector<int> & result);
	//  4d6. drop lowest
	//

	void rollStats_hardMode(WINDOW* parent, int x_offset,
				int y_offset, std::vector<int> & result);
	// 3d6. preassigned.
	//

	std::vector<int> m_roll_result;

};

#endif // !STATS_H
