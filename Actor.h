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

	unsigned char getAttributes() const;
	void setAttributes(unsigned char attrs);
	
	void getXY(short &x, short &y);
	void setXY(short x, short y);
	bool isPenetrable();

	virtual void performAction();

protected:
	Game* m_Game;

	unsigned char m_attributes;
	unsigned char m_avatar;
	bool m_penetrable;
	
	struct {
		short x, y;
	} m_coord;
	
	struct {
		short type, sub;
	} m_descriptor;
	
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
	int getHealthMax();
	void setHealthMax(int n);	

	virtual void performAction();

	/*PLAYER INTERACTIONS*/
	virtual std::string examine();
	
 private:
	bool m_alive;
	int m_natural_armor;

	unsigned short m_health_max;
	int m_health;
};

class Dynamics : public Actor
{
public:
  Dynamics(short type, short sub, short x, short y, Game* g,
	   int h = 1, int m = 0, int a = 0);
	virtual void performaction();
	int fillstats(std::vector<Attribute> statset);
	void getstats(std::vector<int> &v);

	int getMana();
	void setMana(int n);
	int getManaMax();
	void setManaMax(int n);
	
	directions getFacing();
	void setFacing(directions d);
	
private:
	std::vector<Attribute> m_statset;
	unsigned short m_mana_max;
	unsigned short m_mana;

	directions m_facing;
};

class Player : public Dynamics
{
public:
        Player(unsigned short weight, unsigned short height,
	       unsigned short level, int health, int mana,
	       std::string name, Game* g, short sub,
	       unsigned short x, unsigned short y);
	Player(unsigned short wt, unsigned short ht, unsigned short lv,
	       std::string n, Game* g);
	Player(Player &rhs);
	~Player();

	int setname(std::string name);
	std::string getname() const;

	void settitle(std::string title);
	std::string gettitle() const;

	void setlevel(int level);
	int getlevel() const;
	
	void setheight(int height);
	int getheight() const;
	void setweight(int weight);
	int getweight() const;

private:
	std::string m_name;
	std::string m_title;
	
	unsigned short m_height;
	unsigned short m_weight;

	unsigned short m_level;
};

class Mob : public Dynamics
{
 public:
  Mob(short sub, short x, short y, Game* g, int h, int m, int a);
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
