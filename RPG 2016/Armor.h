#pragma once
#include "Item.h"
class Armor : public Item
{
public:
	Armor();
	Armor(Armor const &_armor);
	~Armor();

	bool Read(std::ifstream &_file);

	virtual void UseArmor();
	virtual void ItemDesc();
	int GetArmorBonus();
	std::string GetArmorType();
private:
	int armorBonus;
	std::string armorType;
};

