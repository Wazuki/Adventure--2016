#include <iostream>
#include <fstream>
#include "Item.h"
#include "Player.h"

std::string ITEM_NAME_LABEL = "name";
std::string TYPE_LABEL = "type";
std::string EFFECT_LABEL = "effect";
std::string COST_LABEL = "cost";

Item::Item()
{
	effect = 0;
	cost = 0;
	type = "";
	name = "";
}

Item::Item(const Item &_item)
{
	//std::cout << "Item copy constructor running!\n";
	name = _item.name;
	type = _item.type;
	effect = _item.effect;
	cost = _item.cost;
}

Item::~Item()
{
}

bool Item::Read(std::ifstream &_file)
{
	std::string label;
	std::string temp;
	_file >> label;
	_file.ignore(1);
	std::getline(_file, temp);
	name = temp;

	if (label.compare(ITEM_NAME_LABEL) != 0)
	{
		std::cout << "Label \'" << ITEM_NAME_LABEL << "\' expected\n";
		return false;
	}

	_file >> label;
	_file.ignore(1);
	std::getline(_file, temp);
	type = temp;

	if (label.compare(TYPE_LABEL) != 0)
	{
		std::cout << "Label \'" << TYPE_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> effect;

	if (label.compare(EFFECT_LABEL) != 0)
	{
		std::cout << "Label \'" << EFFECT_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> cost;

	if (label.compare(COST_LABEL) != 0)
	{
		std::cout << "Label \'" << COST_LABEL << "\' expected\n";
		return false;
	}
	return true;
}


std::string Item::GetName()
{
	return name;
}

bool Item::UseItem(Player* _player)
{
	bool itemDestroyed = false;
	std::cout << "Item is type: " << type << "\n";
	if (!type.compare("healing")) //Healing potion
	{
		std::cout << "You drink the " << name << " and heal " << effect << " hp!";
		_player->heal(effect);
		return true;
	}
	else if (!type.compare("warp")) // Warp crystal
	{
		std::cout << "You use the "<<name<<", returning you to the city.\n";
		_player->SetXY(11, 15);
		effect--;
		if (effect <= 0) //Destroy crystal if no more uses left
		{
			std::cout << "The " << name << " has shattered!\n";
			return true;
		}
		else
		{
			std::cout << "The " << name << " still has " << effect << " charges remaining.\n";
			return false;
		}
	}
	else if (!type.compare("ration")) //A ration.
	{
		std::cout << "You nibble the hardtack and food, healing " << effect * 3 * _player->GetLevel() << " hp\n";
		_player->heal(effect * 3);
		return true;
	}

	return false;
}

std::string Item::GetType()
{
	return type;
}

int Item::GetCost()
{
	return cost;
}

void Item::ItemDesc()
{
	if (!type.compare("healing"))
	{
		std::cout << "This " << name << " will heal you for " << effect << ". It costs " << cost << " gp.\n";
	}
	else if (!type.compare("warp"))
	{
		std::cout << "This " << name << " will return you to Caer Benowyc. Charges: " << effect << ". It costs " << cost << "gp.\n";
	}
	else if (!type.compare("ration"))
	{
		std::cout << "This " << name << " will heal you for " << effect * 3 << " times your level. It costs " << cost << "gp\n";
	}
}

//Dummy functions for Weapon derivative
void Item::UseWeapon()
{
	std::cout << "Error! This function should not be called! It is a dummy function for handling UseWeapon() by the Weapon class! Something went wrong.\n";
}

int Item::GetMaxDmg()
{
	std::cout << "Error! This function should not be called! It is a dummy function for handling GetMaxDmg() by the Weapon class! Something went wrong.\n";
	return 0;
}

int Item::GetMinDmg()
{
	std::cout << "Error! This function should not be called! It is a dummy function for handling GetMinDmg() by the Weapon class! Something went wrong.\n";
	return 0;
}

bool Item::GetLight()
{
	std::cout << "Error! This function should not be called! It is a dummy function for handling GetLight() by the Weapon class! Something went wrong.\n";
	return false;
}

bool Item::GetTwoHanded()
{
	std::cout << "Error! This function should not be called! It is a dummy function for handling GetTwoHanded() by the Weapon class! Something went wrong.\n";
	return false;
}

//Dummy funcitons for Armor derivative

void Item::UseArmor()
{
	std::cout << "Error! This function should not be called! It is a dummy function for handling UseArmor() by the Armor class! Something went wrong.\n";
}