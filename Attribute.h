#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include "globals.h"

class Attribute
{
public:
	Attribute(attributes att, int lvl, int mod);
	~Attribute();

	std::string getLonghand();
	std::string getShorthand();
	int getBase();
	bool setBase(int level);
	int getMod();
	bool setMod(int amount);

private:
	attributes m_Att;
	std::string m_Longhand;
	std::string m_Shorthand;
	unsigned short m_Base;
	unsigned short m_Mod;
};

#endif // !ATTRIBUTE_H
