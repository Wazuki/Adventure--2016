#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include "Weapon.h"
#include "Item.h"

using std::cout;
using std::string;

std::string WEAPON_NAME_LABEL = "name";
std::string MIN_DMG_LABEL = "minDmg";
std::string MAX_DMG_LABEL = "maxDmg";
std::string LIGHT_LABEL = "light";
std::string TWOHANDED_LABEL = "twoHanded";
std::string WEAPON_COST_LABEL = "cost";

Weapon::Weapon() : Item()
{
	type = "weapon";
	minDmg = 0, maxDmg = 0;
	light = false;
	twoHanded = false;
}

Weapon::Weapon(Weapon const &_weapon)
{
	//cout << "Weapon copy constructor is running.\n";
	name = _weapon.name;
	type = _weapon.type;
	effect = _weapon.effect;
	cost = _weapon.cost;
	minDmg = _weapon.minDmg;
	maxDmg = _weapon.maxDmg;
	light = _weapon.light;
	twoHanded = _weapon.twoHanded;
}

Weapon::~Weapon()
{

}

bool Weapon::Read(std::ifstream &_file)
{
	std::string label;
	std::string temp;
	_file >> label;
	_file.ignore(1);
	std::getline(_file, temp);
	name = temp;
	if (label.compare(WEAPON_NAME_LABEL) != 0)
	{
		std::cout << "Label \'" << WEAPON_NAME_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> minDmg;

	if (label.compare(MIN_DMG_LABEL) != 0)
	{
		std::cout << "Label \'" << MIN_DMG_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> maxDmg;

	if (label.compare(MAX_DMG_LABEL) != 0)
	{
		std::cout << "Label \'" << MAX_DMG_LABEL << "\' expected\n";
		return false;
	}


	_file >> label >> light;
	//cout << "Light is " << light << "\n";
	if (label.compare(LIGHT_LABEL) != 0)
	{
		std::cout << "Label \'" << LIGHT_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> twoHanded;

	if (label.compare(TWOHANDED_LABEL) != 0)
	{
		std::cout << "Label \'" << TWOHANDED_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> cost;

	if (label.compare(WEAPON_COST_LABEL) != 0)
	{
		std::cout << "Label \'" << WEAPON_COST_LABEL << "\' expected\n";
		return false;
	}
	return true;
}

void Weapon::UseWeapon()
{
	std::string isLight, isTwoHanded;
	if (light)
	{
		isLight = "a light weapon";
	}
	else
	{
		isLight = "not a light weapon";
	}

	if (twoHanded)
	{
		isTwoHanded = "a two-handed weapon";
	}
	else
	{
		isTwoHanded = "not a two-handed weapon";
	}

	std::cout << "You see a " << name << " that does " << minDmg << "d" << maxDmg << ". It is "<<isLight<<" and "<<isTwoHanded<<". It is worth "<<cost<<" gold pieces.\n";
}

int Weapon::GetMinDmg()
{
	return minDmg;
}

int Weapon::GetMaxDmg()
{
	return maxDmg;
}

bool Weapon::GetLight()
{
	return light;
}

bool Weapon::GetTwoHanded()
{
	return twoHanded;
}

void Weapon::ItemDesc()
{
	UseWeapon();
}