#pragma once
#include "Item.h"

class Weapon :	public Item
{
public:
	Weapon();
	Weapon(Weapon const &_weapon);
	~Weapon();
	bool Read(std::ifstream &_file);
	void UseWeapon();
	int GetMaxDmg();
	int GetMinDmg();
	bool GetLight();
	bool GetTwoHanded();
	virtual void ItemDesc();
private:
	int minDmg, maxDmg;
	bool light;
	bool twoHanded;
};

