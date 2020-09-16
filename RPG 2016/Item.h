#pragma once
#include <string>
#include <fstream>

class Player;

class Item
{
public:
	Item();
	Item(const Item &_item);
	~Item();
	virtual bool Read(std::ifstream &_file);
	virtual bool UseItem(Player* _player);
	std::string GetType();
	std::string GetName();
	int GetCost();
	virtual void ItemDesc();
	//Virtual functions to be overriden by Weapon class.
	virtual void UseWeapon();
	virtual int GetMaxDmg();
	virtual int GetMinDmg();
	virtual bool GetLight();
	virtual bool GetTwoHanded();

	//Virtual functions to be overridden by Armor class.
	virtual void UseArmor();
protected:
	std::string name;
	std::string type;
	int effect, cost;
};

