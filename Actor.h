#ifndef ACTOR_H
#define ACTOR_H

#include <vector>
#include "globals.h"
#include "Attribute.h"

class Game;

class GraphObject
{
public:
	GraphObject(short type, short sub, short x, short y, Game* g);
	unsigned char getAvatar() const;
	unsigned short getType() const;
	void getXY(short &x, short &y);
	void setXY(short x, short y);
	bool isPenetrable();
protected:
	Game* m_Game;

	unsigned char m_avatar;
	struct {
		short x, y;
	} m_coord;
	struct {
		short type, sub;
	} m_descriptor;
	bool m_penetrable;
};

class Actor : public GraphObject
{
public:
	Actor( short type, short sub, short x, short y, Game* g, int h = 1, int a = 0);
	int getNaturalArmor();
	void setNaturalArmor(int n);
	void modNaturalArmor(int n);
	int getHealth();
	void setHealth(int n);
	void modHealth(int n);
private:
	bool m_alive;
	int m_natural_armor;
	int m_health;
};

class Dynamics : public Actor
{
public:
	Dynamics(short type, short sub, short x, short y, Game* g, int h = 1, int a = 0);
	virtual void performaction();
	int fillstats(std::vector<Attribute> statset);
	void getstats(std::vector<int> &v);
private:
	std::vector<Attribute> m_statset;
};

class Player : public Dynamics
{
public:
	Player(unsigned short weight, unsigned short height, std::string name, Game* g, short sub, unsigned short x, unsigned short y);
	Player(unsigned short wt, unsigned short ht, std::string n, Game* g);
	Player(Player &rhs);
	~Player();

	int setname(std::string name);
	std::string getname() const;

	void settitle(std::string title);
	std::string gettitle() const;

	void setheight(int height);
	int getheight() const;
	void setweight(int weight);
	int getweight() const;

private:
	std::string m_name;
	std::string m_title;

	unsigned short m_height;
	unsigned short m_weight;

};

class Mob : public Dynamics
{
 public:
  Mob(short sub, short x, short y, Game* g, int h, int a);
};

class Statics : public Actor
{
 public:
  Statics(short type, short sub, short x, short y, Game* g, int h, int a);
};

class Terrain : public Statics
{
 public:
  Terrain(short sub, short x, short y, Game* g, int h, int a);
};

class Item : public Statics
{	
};

#endif // !ACTOR_H
