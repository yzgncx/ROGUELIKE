#include "Stats.h"


Stats::Stats(WINDOW* parent, int x_offset, int y_offset)
{
	noecho();
	std::vector<std::string> roll_types = { "House Rules (easy)", "Standard Roll", "Hard Mode" };
	ScrollMenu select_type(parent, roll_types, x_offset, y_offset);
	
	switch (select_type.select())
	{
	case 0:
		rollStats_houseRules(parent, x_offset, y_offset, m_roll_result);
		break;
	case 2:
		rollStats_hardMode(parent, x_offset, y_offset, m_roll_result);
		break;
	case 1:
	default:
		rollStats_standard(parent, x_offset, y_offset, m_roll_result);
		break;
	}
	
}


Stats::~Stats()
{
}


/*
	easy mode:
		roll 4d6 for each stat
		reroll 1s
		drop lowest stat
		quads are worth 21
*/
void Stats::rollStats_houseRules(WINDOW* parent, int x_offset, int y_offset, std::vector<int> & result)
{
	noecho();
	std::vector<std::vector<int>> rolls;
	std::vector<int> results = { 0,0,0,0,0,0 };
	bool comboFlag[6] = { false, false, false, false, false, false, };
	std::vector<std::string> result_string, attr_short = { "STR", "CON", "DEX", "INT", "WIS", "CHA" };

	for (int i = 0; i < 6; i++) {
		std::vector<int> row; // Create an empty row
		for (int j = 0; j < 4; j++) {
			row.push_back(RandBetween(1, 6)); // Add an element (column) to the row
		}
		rolls.push_back(row); // Add the row to the main vector
	}

	WINDOW* display = subwin(parent, 12, 35, x_offset, y_offset);
	WINDOW* subdisplay = subwin(display, 10, 35, x_offset, y_offset);

	box(subdisplay, 0, 0);
	mvwprintw(subdisplay, 1, 2, "ATTR:   D#1 D#2 D#3 D#4  RESULT");
	for (int i = 0; i < 6; i++)
	{

		mvwprintw(subdisplay, i + 3, 2, "ATTR %d ", i + 1);
		if ((rolls[i][0] == 1 && rolls[i][1] == 1 && rolls[i][2] == 1 && rolls[i][3] == 1))
		{
			results[i] = 21;
			comboFlag[i] = true;
			wattron(subdisplay, A_STANDOUT);
		}
		wprintw(subdisplay, " ");
		for (int j = 0; j < 4; j++)
		{
			if (rolls[i][j] == 1) { wattron(subdisplay, A_BOLD); }
			wprintw(subdisplay, " %d  ", rolls[i][j]);
			if (rolls[i][j] == 1) { wattroff(subdisplay, A_BOLD); } //bold if 1 (indicating reroll) 
		}
		if (rolls[i][0] == 1 && rolls[i][1] == 1 && rolls[i][2] == 1 && rolls[i][3] == 1)
			wattroff(subdisplay, A_STANDOUT);
	}
	touchwin(subdisplay);
	wrefresh(subdisplay);
	mvwprintw(display, 10, 3, "PRESS ENTER TO REROLL ");
	wattron(display, A_BOLD); wprintw(display, "1"); wattroff(display, A_BOLD); wprintw(display, "S ");
	wrefresh(display);
	
	int ch;
	while ((ch = getch()) != KEY_ENTER && ch != 10)
	{
	} //do nothing

	touchwin(display);		//
	wclear(display);		// clear everything
	wclear(subdisplay);		//
	wrefresh(display);		//

	bool rerollValid = true;
	while (rerollValid)
	{
		for (int i = 0; i < 6; i++)
		{
			if (comboFlag[i] == true) continue;
			for (int j = 0; j < 4; j++)
			{
				if (rolls[i][j] <= 1) //reroll ones
					rolls[i][j] = RandBetween(1, 6);
			}
		}
		rerollValid = false;
		box(subdisplay, 0, 0);

		mvwprintw(subdisplay, 1, 2, "ATTR:   D#1 D#2 D#3 D#4  RESULT");
		for (int i = 0; i < 6; i++)
		{

			mvwprintw(subdisplay, i + 3, 2, "ATTR %d ", i + 1);
			if (rolls[i][0] == 1 && rolls[i][1] == 1 && rolls[i][2] == 1 && rolls[i][3] == 1) 
			{
				results[i] = 21;
				comboFlag[i] = true;
				wattron(subdisplay, A_STANDOUT);
			}
			wprintw(subdisplay, " ");
			for (int j = 0; j < 4; j++)
			{
				if (rolls[i][j] == 1) //bold if 1
				{
					if (!comboFlag[i])
						rerollValid = true;
					wattron(subdisplay, A_BOLD);
				}
				wprintw(subdisplay, " %d  ", rolls[i][j]);
				if (rolls[i][j] == 1)
					wattroff(subdisplay, A_BOLD);
			}
			if (rolls[i][0] == 1 && rolls[i][1] == 1 && rolls[i][2] == 1 && rolls[i][3] == 1)
				wattroff(subdisplay, A_STANDOUT);
		}
		touchwin(subdisplay);
		wrefresh(subdisplay);
		switch (rerollValid)
		{
		case true:
			mvwprintw(display, 10, 3, "PRESS ENTER TO REROLL ");
			wattron(display, A_BOLD); wprintw(display, "1"); wattroff(display, A_BOLD); wprintw(display, "S ");
			wrefresh(display);
			break;
		case false:
			mvwprintw(display, 10, 9, "PRESS ENTER TO");
			mvwprintw(display, 11, 5, "ELIMINATE LOWEST ROLLS");
			wrefresh(display);
		}
		int ch;
		while ((ch = getch()) != KEY_ENTER && ch != 10)
		{
		} //display screen until ENTER

		touchwin(display);		//
		wclear(display);		// clear everything
		wclear(subdisplay);		//
		wrefresh(display);		//

	}
	for (int i = 0; i < 6; i++)
	{
		if (comboFlag[i]) continue;
		int index = FindSmallest(rolls[i]);
		rolls[i][index] = 0;
	}

	box(subdisplay, 0, 0);
	mvwprintw(subdisplay, 1, 2, "ATTR:   D#1 D#2 D#3 D#4  RESULT");
	for (int i = 0; i < 6; i++)
	{
		mvwprintw(subdisplay, i + 3, 2, "ATTR %d ", i + 1);
		if (comboFlag[i])						//keep combos highlit
			wattron(subdisplay, A_STANDOUT);	//
		wprintw(subdisplay, " ");
		for (int j = 0; j < 4; j++)
		{
			if (rolls[i][j] == 0) // hide if lowest ( set to 0 )
			{
				wattron(subdisplay, A_INVIS);
			}
			wprintw(subdisplay, " %d  ", rolls[i][j]);
			if (rolls[i][j] == 0)
				wattroff(subdisplay, A_INVIS);
		}
		wattroff(subdisplay, A_STANDOUT);
		wrefresh(subdisplay);
	}
	mvwprintw(display, 10, 5, "PRESS ENTER TO CONTINUE");
	touchwin(subdisplay);
	wrefresh(display);
	while ((ch = getch()) != KEY_ENTER && ch != 10)
	{
	} //display screen until ENTER
	
	for (int i = 0; i < 6; i++) //calculate results.
		if (results[i] == 0)
			results[i] = (rolls[i][0] + rolls[i][1] + rolls[i][2] + rolls[i][3]);

	touchwin(display);		//
	wclear(display);		// clear everything
	wclear(subdisplay);		//
	wrefresh(display);		//

	box(subdisplay, 0, 0);
	mvwprintw(subdisplay, 1, 2, "ATTR:   D#1 D#2 D#3 D#4  RESULT");
	for (int i = 0; i < 6; i++)
	{
		mvwprintw(subdisplay, i + 3, 2, "ATTR %d ", i + 1);
		if (comboFlag[i])
			wattron(subdisplay, A_STANDOUT);
		wprintw(subdisplay, " ");
		for (int j = 0; j < 4; j++)
		{
			if (rolls[i][j] == 0) // hide if lowest ( set to 0 )
			{
				wattron(subdisplay, A_INVIS);
			}
			wprintw(subdisplay, " %d  ", rolls[i][j]);
			if (rolls[i][j] == 0)
				wattroff(subdisplay, A_INVIS);
		}
		if (comboFlag[i])
			wattroff(subdisplay, A_STANDOUT);
		wprintw(subdisplay, "   %d", results[i]);
		wrefresh(subdisplay);
	}
	mvwprintw(display, 10, 5, "PRESS ENTER TO CONTINUE");
	touchwin(subdisplay);
	wrefresh(display);

	while ((ch = getch()) != KEY_ENTER && ch != 10) {} //display screen until ENTER

	touchwin(display);		//
	wclear(display);		// clear everything
	wclear(subdisplay);		//
	wrefresh(display);		//

	for (std::vector<int>::iterator it = results.begin(); it != results.end(); it++)
	{
		std::string tmp;
		if (NumDigits(*it) == 1)
			tmp = "0";
		tmp += (std::to_string(*it) + "               "); //buffer of whitespace to make windows appear the same size
		result_string.push_back(tmp);
	}

	//
	//now that we've generated the rolls, we need to apply them
	//luckily, we have a context menu which allows a user to map
	//items in a setA to a setB. we'll use this here.
	std::vector<int> attr_map, applied_map(6);
	A2BMenu apply_rolls(parent, result_string, "ROLLS", attr_short, "ATTRIBUTES", attr_map, x_offset, y_offset);
	apply_rolls.select();
	for (int i = 0; i < 6; i++)
		applied_map[attr_map[i]] = results[i];
	applied_map.swap(result);

	apply_rolls.~A2BMenu();
	clear();
}
// 4d6. if 4*1, 21;
// otherwise reroll 1 
// & throw out low
//


void Stats::rollStats_standard(WINDOW* parent, int x_offset, int y_offset, std::vector<int> & result)
{
	noecho();
	int ch;
	std::vector<int> results = { 0,0,0,0,0,0 };
	std::vector<std::vector<int>> rolls;
	std::vector<std::string> result_string, attr_short = { "STR", "CON", "DEX", "INT", "WIS", "CHA" };

	for (int i = 0; i < 6; i++) {
		std::vector<int> row; // Create an empty row
		for (int j = 0; j < 4; j++) {
			row.push_back(RandBetween(1, 6)); // Add an element (column) to the row
		}
		rolls.push_back(row); // Add the row to the main vector
	}

	WINDOW* display = subwin(parent, 12, 35, x_offset, y_offset);
	WINDOW* subdisplay = subwin(display, 10, 35, x_offset, y_offset);

	box(subdisplay, 0, 0);
	mvwprintw(subdisplay, 1, 2, "ATTR:   D#1 D#2 D#3 D#4  RESULT");
	for (int i = 0; i < 6; i++)
	{

		mvwprintw(subdisplay, i + 3, 2, "ATTR %d ", i + 1);
		wprintw(subdisplay, " ");
		for (int j = 0; j < 4; j++)
			wprintw(subdisplay, " %d  ", rolls[i][j]);
	}
	touchwin(subdisplay);
	wrefresh(subdisplay);
	mvwprintw(display, 10, 9, "PRESS ENTER TO");
	mvwprintw(display, 11, 5, "ELIMINATE LOWEST ROLLS");
	wrefresh(display);

	while ((ch = getch()) != KEY_ENTER && ch != 10)
	{
	} //display screen until ENTER

	touchwin(display);		//
	wclear(display);		// clear everything
	wclear(subdisplay);		//
	wrefresh(display);		//

	for (int i = 0; i < 6; i++)
	{
		int index = FindSmallest(rolls[i]);
		rolls[i][index] = 0;
	}

	box(subdisplay, 0, 0);
	mvwprintw(subdisplay, 1, 2, "ATTR:   D#1 D#2 D#3 D#4  RESULT");
	for (int i = 0; i < 6; i++)
	{
		mvwprintw(subdisplay, i + 3, 2, "ATTR %d ", i + 1);
		wprintw(subdisplay, " ");
		for (int j = 0; j < 4; j++)
		{
			if (rolls[i][j] == 0) // hide if lowest ( set to 0 )
				wattron(subdisplay, A_INVIS);
			wprintw(subdisplay, " %d  ", rolls[i][j]);
			if (rolls[i][j] == 0) // hide if lowest ( set to 0 )
				wattroff(subdisplay, A_INVIS);
		}
		wattroff(subdisplay, A_STANDOUT);
		wrefresh(subdisplay);
	}
	mvwprintw(display, 10, 5, "PRESS ENTER TO CONTINUE");
	touchwin(subdisplay);
	wrefresh(display);
	while ((ch = getch()) != KEY_ENTER && ch != 10)
	{
	} //display screen until ENTER

	for (int i = 0; i < 6; i++) //calculate results.
		if (results[i] == 0)
			results[i] = (rolls[i][0] + rolls[i][1] + rolls[i][2] + rolls[i][3]);

	touchwin(display);		//
	wclear(display);		// clear everything
	wclear(subdisplay);		//
	wrefresh(display);		//

	box(subdisplay, 0, 0);
	mvwprintw(subdisplay, 1, 2, "ATTR:   D#1 D#2 D#3 D#4  RESULT");
	for (int i = 0; i < 6; i++)
	{
		mvwprintw(subdisplay, i + 3, 2, "ATTR %d ", i + 1);
		wprintw(subdisplay, " ");
		for (int j = 0; j < 4; j++)
		{
			if (rolls[i][j] == 0) // hide if lowest ( set to 0 )
			{
				wattron(subdisplay, A_INVIS);
			}
			wprintw(subdisplay, " %d  ", rolls[i][j]);
			if (rolls[i][j] == 0)
				wattroff(subdisplay, A_INVIS);
		}
		wprintw(subdisplay, "   %d", results[i]);
		wrefresh(subdisplay);
	}
	mvwprintw(display, 10, 5, "PRESS ENTER TO CONTINUE");
	touchwin(subdisplay);
	wrefresh(display);

	while ((ch = getch()) != KEY_ENTER && ch != 10) {} //display screen until ENTER

	touchwin(display);		//
	wclear(display);		// clear everything
	wclear(subdisplay);		//
	wrefresh(display);		//

	for (std::vector<int>::iterator it = results.begin(); it != results.end(); it++)
	{
		std::string tmp;
		if (NumDigits(*it) == 1)
			tmp = "0";
		tmp += (std::to_string(*it) + "               "); //buffer of whitespace to make windows appear the same size
		result_string.push_back(tmp);
	}

	std::vector<int> attr_map;
	A2BMenu apply_rolls(parent, result_string, "ROLLS", attr_short, "ATTRIBUTES", attr_map, x_offset, y_offset);
	apply_rolls.select();
	attr_map.swap(result);

	apply_rolls.~A2BMenu();
	clear();
}

void Stats::rollStats_hardMode(WINDOW* parent, int x_offset, int y_offset, std::vector<int> & result)
{
	noecho();
	int ch;
	std::vector<int> results = { 0,0,0,0,0,0 };
	std::vector<std::vector<int>> rolls;
	std::vector<std::string> result_string, attr_short = { "STR", "CON", "DEX", "INT", "WIS", "CHA" };

	for (int i = 0; i < 6; i++) {
		std::vector<int> row; // Create an empty row
		for (int j = 0; j < 3; j++) {
			row.push_back(RandBetween(1, 6)); // Add an element (column) to the row
		}
		rolls.push_back(row); // Add the row to the main vector
	}

	WINDOW* display = subwin(parent, 12, 35, x_offset, y_offset);
	WINDOW* subdisplay = subwin(display, 10, 35, x_offset, y_offset);

	box(subdisplay, 0, 0);
	mvwprintw(subdisplay, 1, 2, "ATTR:   D#1   D#2   D#3  RESULT");
	for (int i = 0; i < 6; i++)
	{

		mvwprintw(subdisplay, i + 3, 2, "%s   ", attr_short[i].c_str());
		wprintw(subdisplay, " ");
		for (int j = 0; j < 3; j++)
			wprintw(subdisplay, " %d    ", rolls[i][j]);
	}
	touchwin(subdisplay);
	wrefresh(subdisplay);
	mvwprintw(display, 10, 5, "PRESS ENTER TO CONTINUE");
	wrefresh(display);

	while ((ch = getch()) != KEY_ENTER && ch != 10)
	{
	} //display screen until ENTER

	for (int i = 0; i < 6; i++) //calculate results.
		if (results[i] == 0)
			results[i] = (rolls[i][0] + rolls[i][1] + rolls[i][2]);

	touchwin(display);		//
	wclear(display);		// clear everything
	wclear(subdisplay);		//
	wrefresh(display);		//

	box(subdisplay, 0, 0);
	mvwprintw(subdisplay, 1, 2, "ATTR:   D#1   D#2   D#3  RESULT");
	for (int i = 0; i < 6; i++)
	{
		mvwprintw(subdisplay, i + 3, 2, "%s   ", attr_short[i].c_str());
		wprintw(subdisplay, " ");
		for (int j = 0; j < 3; j++)
		{
			if (rolls[i][j] == 0) // hide if lowest ( set to 0 )
			{
				wattron(subdisplay, A_INVIS);
			}
			wprintw(subdisplay, " %d    ", rolls[i][j]);
			if (rolls[i][j] == 0)
				wattroff(subdisplay, A_INVIS);
		}
		wprintw(subdisplay, " %d", results[i]);
		wrefresh(subdisplay);
	}
	mvwprintw(display, 10, 5, "PRESS ENTER TO CONTINUE");
	touchwin(subdisplay);
	wrefresh(display);

	while ((ch = getch()) != KEY_ENTER && ch != 10) {} //display screen until ENTER

	touchwin(display);		//
	wclear(display);		// clear everything
	wclear(subdisplay);		//
	wrefresh(display);		//
	result.swap(results);
}