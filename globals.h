
#ifndef GLOBALS_H
#define GLOBALS_H

#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <set>


const int OOB = -1;
const int DUMMY = -1;

const int MASCULINE = 0;
const int FEMININE = 1;

//==========================//
//    entity identifiers    //
//==========================//
const char PLAYER = '\1';
const char ITEM = '\2';
const char MOB = '\3';
const char TERRAIN = '\4';


enum class mobs
{
        DUMMY = 0,
        CHICKEN = 1
};

enum class attributes 
{
	STRENGTH = 0,
	CONSTITUTION = 1,
	DEXTERITY = 2,
	INTELLIGENCE = 3,
	WISDOM = 4,
	CHARISMA = 5
};

enum class directions
{
        N = 0,
	W = 1,
	S = 2,
	E = 3  
};

//=======================//
//    exception codes    //
//=======================//
typedef const short int err_t;
err_t BAD_RANGE = 1001;
err_t BAD_FILE = 1002;

//==========================================//
//    player savefile variable locations    //
//==========================================//

const int P_NAME = 0;
const int P_GENDER = 1;
const int P_HEIGHT = 2;
const int P_WEIGHT = 3;


//======================================//
//     utility function declarations    //
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

//======================//
//    multimap class    //
//======================//


template <class T1, class T2>
class Multimap
{
 public:  
  typedef typename std::map<T1,std::set<T2>>::iterator iterator;
  typedef typename std::set<T2>::iterator set_iterator;
  iterator begin() { return m_Multimap.begin(); }
  set_iterator set_begin(iterator it) {return it->second.begin();}  
  iterator end() { return m_Multimap.end(); }
  set_iterator set_end(iterator it) { return it->second.end(); }
  

  void insert(T1 key, T2 elem);
  
  // returns 0 if suzzessfully removed. retuens 1 if key not found
  // returns 2 if elem not found in key.
  int remove(T1 key, T2 elem);

  // returns 0 if successfully moved from old_key to new_key.
  // returns 1 if no old_key; returns 2 if no elem at old_key
  int reassign(T1 old_key, T1 new_key, T2 elem);

  iterator find_set(T1 key);
  set_iterator find(iterator s_it, T2 elem);
  
 private:
  std::map<T1, std::set<T2>> m_Multimap;
};



//===========================
//    multimap functions
//===========================


template <class T1, class T2>
void Multimap<T1,T2>::insert(T1 key, T2 elem)
{
  typename std::map<T1, std::set<T2>>::iterator it = m_Multimap.find(key);
  if(it == m_Multimap.end()) {
    std::set<T2> tmp;
    tmp.insert(elem);
    m_Multimap.insert(std::pair<T1,std::set<T2>>(key,tmp));
  }
  else {
    it->second.insert(elem);
  }
}

template <class T1, class T2>
int Multimap<T1,T2>::remove(T1 key, T2 elem)
{
  typename std::map<T1, std::set<T2>>::iterator k_it = m_Multimap.find(key);
  if(k_it == m_Multimap.end()) {
    return 1;
  }
  else {
    size_t removed = k_it->erase(elem);
    return (removed == 1) ? 0 : 2; 
  }
}

template <class T1, class T2>
int Multimap<T1,T2>::reassign(T1 old_key, T1 new_key, T2 elem)
{
  int res = this->remove(old_key,elem);
  if(res == 0) {
    this->insert(new_key, elem);
  }
  return res;
}

template <class T1, class T2>
typename Multimap<T1,T2>::iterator Multimap<T1,T2>::find_set(T1 key)
{
  return m_Multimap.find(key);
}

template <class T1, class T2>
typename Multimap<T1,T2>::set_iterator Multimap<T1,T2>::find(typename Multimap<T1,T2>::iterator s_it, T2 elem)
{
  return s_it->find(elem);
}


//================//
//    typedefs    //
//================//
class Actor;
typedef Multimap<std::pair<short,short>,Actor*> go_mmap;


#endif // !GLOBALS_H
