#include <iostream>
#include "Armor.h"
#include "Player.h"

const std::string ARMOR_NAME_LABEL = "name";
const std::string ARMOR_TYPE_LABEL = "armorType";
const std::string ARMOR_BONUS_LABEL = "armorBonus";
const std::string ARMOR_COST_LABEL = "cost";

Armor::Armor() : Item()
{
	type = "armor";
	armorBonus = 0;
	armorType = "";
}

Armor::Armor(Armor const &_armor)
{
	//std::cout << "Armor copy constructor is running.\n";
	name = _armor.name;
	type = _armor.type;
	armorBonus = _armor.armorBonus;
	armorType = _armor.armorType;
	effect = _armor.effect;
	cost = _armor.cost;
}

Armor::~Armor()
{
}

void Armor::UseArmor()
{
	std::cout << "You see a suit of " << name << " armor. It is "<<armorType<<" armor and gives a +"<<armorBonus<<" to AC. It is worth " << cost << " gold pieces.\n";
}

void Armor::ItemDesc()
{
	//std::cout << "Armor UseItem() is calling UseArmor().\n";
	UseArmor();
}

bool Armor::Read(std::ifstream &_file)
{
	std::string label;
	std::string temp;
	_file >> label;
	_file.ignore(1);
	std::getline(_file, temp);
	name = temp;
	if (label.compare(ARMOR_NAME_LABEL) != 0)
	{
		std::cout << "Label \'" << ARMOR_NAME_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> armorType;

	if (label.compare(ARMOR_TYPE_LABEL) != 0)
	{
		std::cout << "Label \'" << ARMOR_TYPE_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> armorBonus;

	if (label.compare(ARMOR_BONUS_LABEL) != 0)
	{
		std::cout << "Label \'" << ARMOR_BONUS_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> cost;

	if (label.compare(ARMOR_COST_LABEL) != 0)
	{
		std::cout << "Label \'" << ARMOR_COST_LABEL << "\' expected\n";
		return false;
	}

	return true;
}


int Armor::GetArmorBonus()
{
	return armorBonus;
}

std::string Armor::GetArmorType()
{
	return armorType;
}