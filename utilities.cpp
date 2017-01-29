#include "globals.h"
#include <cstdlib>

int NumDigits(int x) //thanks brad.
{
	x = abs(x);
	return (x < 10 ? 1 :
		(x < 100 ? 2 :
			(x < 1000 ? 3 :
				(x < 10000 ? 4 :
					(x < 100000 ? 5 :
						(x < 1000000 ? 6 :
							(x < 10000000 ? 7 :
								(x < 100000000 ? 8 :
									(x < 1000000000 ? 9 :
										10)))))))));
}

int FindSmallest(std::vector<int> vector)		//return index of (last) smallest item
{
	int smallest_index;
	if (vector.size() == 0) return -1;
	for (int i = 0; static_cast<size_t>(i) < vector.size(); i++)
	{
		if (i == 0)
			smallest_index = i;
		if (vector[i] <= vector[smallest_index])
			smallest_index = i;
	}
	return smallest_index;

}


int D4() { return (rand() % 4) + 1; }

int D6() { return (rand() % 6) + 1; }

int D8() { return (rand() % 8) + 1; }

int D10() { return (rand() % 10) + 1; }

int D12() { return (rand() % 12) + 1; }

int D20() { return (rand() % 20) + 1; }

int D100() { return (rand() % 100) + 1; }

int Rand()
{
	return rand();
}

int RandBetween(int low, int high)
{
	if (high < low)
	{
		throw BAD_RANGE;
	}
	return low + (rand() % (int)(high - low + 1));
}

int RandNTHBetween(int low, int high, int mod)
{
	if (high < low)
	{
		throw BAD_RANGE;
	}
	return ((rand() % (high - low)) + low) / mod;
}

