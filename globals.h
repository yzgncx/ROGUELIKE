
#ifndef GLOBALS_H
#define GLOBALS_H

#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

const int OOB = -1;
const int DUMMY = -1;

const int MASCULINE = 0;
const int FEMININE = 1;

//==============================//
//	   entity identifiers		//
//==============================//
const char PLAYER = '\1';
const char ITEM = '\2';
const char MOB = '\3';
const char TERRAIN = '\4';
 

enum class attributes 
{
	STRENGTH = 0,
	CONSTITUTION = 1,
	DEXTERITY = 2,
	INTELLIGENCE = 3,
	WISDOM = 4,
	CHARISMA = 5
};


//==============================//
//		exception codes			//
//==============================//
typedef const short int err_t;
err_t BAD_RANGE = 1001;
err_t BAD_FILE = 1002;

//==============================================//
//		player savefile variable locations		//
//==============================================//

const int P_NAME = 0;
const int P_GENDER = 1;
const int P_HEIGHT = 2;
const int P_WEIGHT = 3;


//======================================//
//	  utility function declarations		//
//======================================//

int NumDigits(int x);

int FindSmallest(std::vector<int> vector);


int D4();
int D6();
int D8();
int D10();
int D12();
int D20();
int D100();
int Rand();
int RandBetween(int low, int high);
int RandNTHBetween(int low, int high, int mod);



#endif // !GLOBALS_H
