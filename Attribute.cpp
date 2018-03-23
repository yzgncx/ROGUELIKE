#include "Attribute.h"



Attribute::Attribute(attributes att, int lvl, int mod)
{
	m_Att = att; 
	m_Base = lvl; 
	m_Mod = mod;
	switch (att) 
	{
	case attributes::STRENGTH:
		m_Longhand = "STRENGTH"; m_Shorthand = "STR";
		break;
	case attributes::CONSTITUTION:
		m_Longhand = "CONSTITUTION"; m_Shorthand = "CON";
		break;
	case attributes::DEXTERITY:
		m_Longhand = "DEXTERITY"; m_Shorthand = "DEX";
		break;
	case attributes::INTELLIGENCE:
		m_Longhand = "INTELLIGENCE"; m_Shorthand = "INT";
		break;
	case attributes::WISDOM:
		m_Longhand = "WISDOM"; m_Shorthand = "WIS";
		break;
	case attributes::CHARISMA:
		m_Longhand = "CHARISMA"; m_Shorthand = "CHA";
		break;
	}
}


Attribute::~Attribute()
{
}

std::string Attribute::getLonghand() { return m_Longhand; }

std::string Attribute::getShorthand() { return m_Shorthand; }

int Attribute::getBase() { return m_Base; }

bool Attribute::setBase(int level)
{
	if (level >= 0 && level < 100) {
		m_Base = level;
		return true;
	}
	return false;
}

int Attribute::getMod() { return m_Mod; }

bool Attribute::setMod(int amount)
{
	if (amount > -100 && amount < 100) {
		m_Mod = amount;
		return true;
	}
	return false;
}