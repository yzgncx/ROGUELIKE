#include "Actor.h"

//==================================//
//		GRAPHOBJECT FUNCTIONS		//
//==================================//
GraphObject::GraphObject( short type, short sub, short x, short y, Game* g) : m_Game(g)
{
	m_coord.x = x; m_coord.y = y;
	m_descriptor.type = type;
	m_descriptor.sub = sub;
	m_penetrable = true;

	switch (type)
	{
	case 0:
		//dummy
		return;
	case PLAYER:
		//player
		m_avatar = 0xA0;
		break;
	case ITEM:
		//item
		break;
	case MOB:
		//NPC
		break;
	case TERRAIN:
		//terrain
		break;
	default:
		break;
	}
}

void GraphObject::getXY(short &x, short &y) { x = m_coord.x; y = m_coord.y; }

void GraphObject::setXY(short x, short y) { m_coord.x = x; m_coord.y = y; }

bool GraphObject::isPenetrable() { return m_penetrable; }

unsigned char GraphObject::getAvatar() const { return m_avatar; }

unsigned short GraphObject::getType() const { return m_descriptor.type; }

//==================================//
//		ACTOR	 FUNCTIONS	    	//
//==================================//
Actor::Actor(short type, short sub, short x, short y, Game* g, int h, int a ) : GraphObject(type, sub, x, y, g)
{
	m_alive = true;
	m_health = h;
	m_natural_armor = a;
}

int Actor::getNaturalArmor() { return m_natural_armor; }
void Actor::setNaturalArmor(int n) { m_natural_armor = n; }
void Actor::modNaturalArmor(int n) { m_natural_armor += n; }
int Actor::getHealth() { return m_health; }
void Actor::setHealth(int n) { m_health = n; }
void Actor::modHealth(int n) { m_health += n; }

//==================================//
//		DYNAMICS FUNCTIONS	    	//
//==================================//
Dynamics::Dynamics(short type, short sub, short x, short y, Game* g, int h, int a) : Actor(type, sub, x, y, g, h, a) {}

void Dynamics::performaction() {}

int Dynamics::fillstats(std::vector<Attribute> statset)
{
	if (statset.size() != 6) return -1;
	std::vector<Attribute> tmp;
	for (int i = 0; i < 6; i++)
		tmp.push_back(statset[i]);
	m_statset.swap(tmp);
	return 0;
}

void Dynamics::getstats(std::vector<int> &v)
{
	std::vector<int> result;
	for (int i = 0; i < 6; i++)
		result.push_back(m_statset[i].getBase());
	result.swap(v);
}




//==================================//
//		  PLAYER FUNCTIONS	    	//
//==================================//
Player::Player(unsigned short weight, unsigned short height, std::string name, Game* g,
	short sub, unsigned short x, unsigned short y) : Dynamics(PLAYER, sub, x, y, g)
{
	m_weight = weight;
	m_height = height;
	m_name = name;
}

Player::Player(unsigned short wt, unsigned short ht, std::string n, Game* g) : Dynamics(PLAYER, 0, OOB, OOB, g) 
{
	m_weight = wt;
	m_height = ht;
	m_name = n;
}

Player::Player(Player &rhs) : Dynamics(PLAYER, rhs.m_descriptor.sub, rhs.m_coord.x, rhs.m_coord.y, rhs.m_Game)
{}

Player::~Player()
{
}

int Player::setname(std::string name)
{ // inform player of restriction at time of name-set
	if (name.length() == 0)
		return 2;
	else if (name.length() <= 16) {
		m_name = name;
		return 0;
	}
	else {
		return 1;
	}
}

std::string Player::getname() const { return m_name; }

void Player::settitle(std::string title) { m_title = title; }

std::string Player::gettitle() const { return m_title; }

void Player::setheight(int height) { m_height = height; }

int Player::getheight() const { return m_height; }

void Player::setweight(int weight) { m_weight = weight; }

int Player::getweight() const { return m_weight; }

//==================================//
//		  MOB	FUNCTIONS	    	//
//==================================//
//
//
//
//


//==================================//
//		 STATICS FUNCTIONS	    	//
//==================================//

Statics::Statics(short type, short sub, short x, short y, Game* g, int h, int a) : Actor(type, sub, x, y, g, h, a) {}


//==================================//
//		 TERRAIN FUNCTIONS	    	//
//==================================//

Terrain::Terrain(short sub, short x, short y, Game* g, int h, int a) : Statics(TERRAIN, sub, x, y, g, h, a)
{
	switch (sub)
	{
	case 0: //temporary
		m_avatar = 0xAB;
		m_penetrable = false;
	default:
		break;
	}
}