#include "Game.h"
using namespace std;

Game::Game()
{
}


Game::~Game()
{
	delete this->m_Player;
}

void Game::newGame()
{
	initscr();

	//
	//TODO: intro (using same name overwrites previous save file)
	//

	string path, filename;
	char p_name[100];
	ofstream outFile;

	bool nameTripwire = false;		//to get text to line up. surely there's a better way.
	while (checkName(filename) != 0) {
		mvprintw(8, 1, "PRESS ENTER TO CONTINUE");
		mvprintw(nameTripwire, 0, " What is your name? : ");	//nameT.. == 0 if false, 1 if true
		refresh();
		getnstr(p_name, sizeof(p_name) - 1);
		
		filename = p_name;
		size_t s = filename.size();
			for (size_t i = 0; i < s; i++)
			{	//illegal characters in filename replaced with '_'
				if (filename[i] == '\\' || filename[i] == '/' || filename[i] == ':'
					|| filename[i] == '*' || filename[i] == '?' || filename[i] == '"'
					|| filename[i] == '<' || filename[i] == '>' || filename[i] == '|')
					filename[i] = '_';
			}
		clear();
		
		switch (checkName(filename)) {		// 0 == good name // 1 == too long // 2 == no name
		case 0:
			path = "save/";
			path.append(filename.c_str());
			m_filename = filename;
			outFile.open(path);				// creates file with name filename
			outFile << p_name << endl;		// outFile LINE 01
			break;
		case 1:
			printw(" Maybe something shorter? (16 char limit)\n");
			nameTripwire = true;
			refresh();
			break;
		case 2:
			printw(" Surely you must have a name.\n");
			nameTripwire = true;
			refresh();
			break;
		}
	}
	clear();

	//
	//TODO: completely random character? if yes return all stats printed
	//

	printw(" Tell me a bit about yourself.\n Are you: ");
	mvprintw(8, 1, "USE W & S TO TOGGLE MENUS, R FOR RANDOM VALUE, ENTER TO SELECT"); // TODO: move toward WASD formatting
	
	//
	// TODO: randomize gender? CLEANUP
	//

	std::vector<std::string> gender_vec; 
	gender_vec.push_back("MASCULINE");
	gender_vec.push_back("FEMININE");

	ScrollMenu gender(stdscr, gender_vec, 3, 3);
	switch (gender.select(true))			//outFile LINE 02
	{
	case 0:
		if (!outFile.bad())
		outFile << MASCULINE << endl;
		break;
	case 1:
		if (!outFile.bad())
		outFile << FEMININE << endl;
		break;
	}
	
	//	gender.~ScrollMenu();
	clear();
	refresh();

	printw(" How tall are you? \n");
	mvprintw(8, 1, "USE W/S TO TOGGLE UP/DOWN, R FOR RANDOM VALUE, ENTER TO SELECT");

	SliderMenu height(stdscr, "HEIGHT (cm)", 220, 140, 180, 1, 3, 3);
	int p_height = height.select(true);
	if (!outFile.bad())
		outFile << (p_height) << endl;		//outFile LINE 03

	height.~SliderMenu();
	clear();
	refresh();

	printw(" What do you weigh? \n");
	mvprintw(8, 1, "USE W/S TO TOGGLE UP/DOWN, R FOR RANDOM VALUE, ENTER TO SELECT");

	SliderMenu weight(stdscr, "WEIGHT (kg)", 200, 30, 70, 1, 3, 3);
	int p_weight = weight.select(true);
	if (!outFile.bad())
		outFile << (p_weight) << endl;		//outFile LINE 03
	weight.~SliderMenu();
	clear();
	refresh();

	m_Player = new Player(p_weight, p_height, p_name, this);
	std::vector<Attribute> statset;
	generatestats(stdscr, 3, 3, statset);
	m_Player->fillstats(statset);

	std::vector<int> stats;
	m_Player->getstats(stats);

	if (!outFile.bad())
	{
		for (std::vector<int>::iterator it = stats.begin(); it != stats.end(); it++)
		{	
			outFile << to_string(*it).c_str() << endl;	//lines 4-10 are for stat attributes, in the standard order.
		}
	}
	outFile.close();

	endwin();
	return;
}

void Game::generatestats(WINDOW* parent, int x_offset, int y_offset, std::vector<Attribute> &statset)
{
	Stats s(parent, x_offset, y_offset);
	for (int i = 0; i < 6; i++)
	{
		attributes att;
		switch (i)
		{
		case 0: att = attributes::STRENGTH;	break;
		case 1: att = attributes::CONSTITUTION; break;
		case 2: att = attributes::DEXTERITY; break;
		case 3: att = attributes::INTELLIGENCE; break;
		case 4: att = attributes::WISDOM; break;
		case 5: att = attributes::CHARISMA;	break;
		}
		Attribute temp(att, s.m_roll_result[i], 0);
		statset.push_back(temp);
	}
}


void Game::loadGame() //will need filepath at some point
{
  
}

int Game::checkName(std::string name)
{ // inform player of restriction at time of name-set
	if (name.length() == 0)
		return 2;
	else if (name.length() <= 16)
		return 0;
	else
		return 1;
}


std::string Game::getFilename() { return m_filename; }

Player* Game::getPlayer() { return m_Player; }
